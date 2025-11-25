"""
Dataset converter between AIL and COCO formats.

This module provides functionality to convert between AIL (Aurora Imaging Library)
and COCO (Common Objects in Context) dataset formats for object detection tasks.

AIL format uses WKT (Well-Known Text) for bounding box representation,
while COCO uses a simple [x_min, y_min, width, height] format.
"""

import json, os, argparse

import ail11 as ail


def coco_bbox_from_wkt(wkt_string):
    """
    Convert WKT polygon string to COCO bbox format.

    Args:
        wkt_string (str): WKT polygon string in format "POLYGON((x1 y1,x2 y2,...))"

    Returns:
        tuple: COCO bbox format (x_min, y_min, width, height)
    """
    # Extract coordinates from the WKT string
    coords = wkt_string.replace("POLYGON((", "").replace("))", "").split(",")
    coords = [tuple(map(float, coord.split())) for coord in coords]

    # Compute the COCO bbox format
    x_min = min(coord[0] for coord in coords)
    y_min = min(coord[1] for coord in coords)
    x_max = max(coord[0] for coord in coords)
    y_max = max(coord[1] for coord in coords)

    w = x_max - x_min
    h = y_max - y_min

    return (x_min, y_min, w, h)


def wkt_from_coco_bbox(bbox):
    """
    Convert COCO bbox format to WKT polygon string.

    Args:
        bbox (tuple): COCO bbox format (x_min, y_min, width, height)

    Returns:
        str: WKT polygon string
    """
    x_min, y_min, w, h = bbox
    x_max = x_min + w
    y_max = y_min + h
    return f"POLYGON(({x_min} {y_min},{x_min} {y_max},{x_max} {y_max},{x_max} {y_min},{x_min} {y_min}))"


def inquire_image_dimensions(im_path):
    """
    Get image dimensions using AIL library.

    Args:
        im_path (str): Path to the image file

    Returns:
        tuple: (width, height) of the image
    """
    im = ail.MbufRestore(im_path, ail.M_DEFAULT_HOST)
    width = ail.MbufInquire(im, ail.M_SIZE_X)
    height = ail.MbufInquire(im, ail.M_SIZE_Y)
    ail.MbufFree(im)
    return (width, height)


def AIL_to_COCO(ail_json_path, output_path):
    """
    Convert AIL dataset format to COCO format.

    Args:
        ail_json_path (str): Path to the AIL JSON file
        output_path (str): Path where COCO JSON will be saved
    """
    if not os.path.exists(ail_json_path):
        raise FileNotFoundError(f"AIL JSON file not found: {ail_json_path}")

    coco_format = {"images": [], "annotations": [], "categories": []}
    category_id_mapping = {}

    with open(ail_json_path, "r") as f:
        ail_json = json.load(f)

    # Map AIL entries to COCO format
    for entry in ail_json["entries"]:
        image_id = len(coco_format["images"]) + 1
        image_path = os.path.join(os.path.dirname(ail_json_path), entry["image_name"])
        image_width, image_height = inquire_image_dimensions(image_path)

        coco_format["images"].append(
            {
                "id": image_id,
                "file_name": entry["image_name"],
                "width": image_width,
                "height": image_height,
            }
        )

        for annotation in entry["annotations"]:
            category = annotation["character"]
            if category not in category_id_mapping:
                category_id_mapping[category] = len(category_id_mapping) + 1

            bbox = coco_bbox_from_wkt(annotation["bbox"])
            bbox_area = bbox[2] * bbox[3]  # w * h

            coco_format["annotations"].append(
                {
                    "id": len(coco_format["annotations"]) + 1,
                    "image_id": image_id,
                    "category_id": category_id_mapping[category],
                    "segmentation": [],
                    "area": bbox_area,
                    "bbox": bbox,
                    "iscrowd": 0,
                }
            )

    # Create categories from the mapping
    coco_format["categories"] = [
        {"id": cat_id, "name": cat} for cat, cat_id in category_id_mapping.items()
    ]

    with open(output_path, "w") as f:
        json.dump(coco_format, f, indent=2)


def COCO_to_AIL(coco_dataset_path, output_path):
    """
    Convert COCO dataset format to AIL format.

    Args:
        coco_dataset_path (str): Path to the COCO JSON file
        output_path (str): Path where AIL JSON will be saved
    """
    if not os.path.exists(coco_dataset_path):
        raise FileNotFoundError(f"COCO JSON file not found: {coco_dataset_path}")

    with open(coco_dataset_path, "r") as f:
        coco_data = json.load(f)

    ail_format_entries = {}
    image_id_mapping = {img["id"]: img["file_name"] for img in coco_data["images"]}
    cat_id_to_name = {cat["id"]: cat["name"] for cat in coco_data["categories"]}

    for ann in coco_data["annotations"]:
        image_id = ann["image_id"]
        if image_id not in ail_format_entries:
            ail_format_entries[image_id] = {
                "image_name": image_id_mapping[image_id],
                "annotations": [],
            }
        ail_format_entries[image_id]["annotations"].append(
            {
                "character": cat_id_to_name[ann["category_id"]],
                "bbox": wkt_from_coco_bbox(ann["bbox"]),
            }
        )

    ail_format = {"entries": list(ail_format_entries.values())}

    with open(output_path, "w") as f:
        json.dump(ail_format, f, indent=2)


def parse_command_line_args():
    parser = argparse.ArgumentParser(description="Convert AIL dataset to COCO format")
    parser.add_argument("input", help="Path to the input file")
    parser.add_argument(
        "input_format",
        help="Format of the input file (AIL or COCO)",
        choices=["AIL", "COCO"],
        default="AIL",
    )
    parser.add_argument("output", help="Path to the output file")
    parser.add_argument(
        "output_format",
        help="Format of the output file (AIL or COCO)",
        choices=["AIL", "COCO"],
        default="COCO",
    )

    return parser.parse_args()


def main():
    ail_app = ail.MappAlloc(ail.M_NULL, ail.M_DEFAULT)

    try:
        args = parse_command_line_args()

        if args.input_format == "AIL" and args.output_format == "COCO":
            AIL_to_COCO(args.input, args.output)
        elif args.input_format == "COCO" and args.output_format == "AIL":
            COCO_to_AIL(args.input, args.output)
        else:
            print("Unsupported conversion.")
    except Exception as e:
        print(f"Error occurred: {e}")

    ail.MappFree(ail_app)


if __name__ == "__main__":
    main()
