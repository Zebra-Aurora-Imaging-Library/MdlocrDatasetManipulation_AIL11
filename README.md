# AIL DLOCR ↔ COCO Dataset Converter

This script converts datasets between the AIL DLOCR (Aurora Imaging Library Deep Learning OCR) and COCO (Common Objects in Context) formats.

## Dataset Formats

### AIL DLOCR Format

The AIL DLOCR format uses a JSON file to define the dataset. Here's a quick look at its structure:

```json
{
  "entries": [
    {
      "image_name": "path/to/image.jpg",
      "annotations": [
        {
          "character": 1,
          "bbox": "POLYGON((604.735 252.293,604.735 315.632,647.279 315.632,647.279 252.293,604.735 252.293))"
        }
      ]
    }
  ]
}
```

### COCO Format

The COCO format also uses a JSON file but organizes the data into `images`, `annotations`, and `categories`.

```json
{
  "images": [
    {
      "id": 1,
      "file_name": "image001.jpg",
      "width": 1430,
      "height": 567
    }
  ],
  "annotations": [
    {
      "id": 1,
      "image_id": 1,
      "category_id": 1,
      "bbox": [604.735, 252.293, 42.544, 63.339],
      "area": 2694.69,
      "segmentation": [],
      "iscrowd": 0
    },
    {
      "id": 2,
      "image_id": 1,
      "category_id": 2,
      "bbox": [660.237, 251.441, 19.53, 65.489],
      "area": 1279.00,
      "segmentation": [],
      "iscrowd": 0
    }
  ],
  "categories": [
    {
      "id": 1,
      "name": "5"
    },
    {
      "id": 2,
      "name": "1"
    }
  ]
}
```

## How to Use

You can run this converter from the command line.

### Command

```bash
python convert_dataset.py <input_file> <input_format> <output_file> <output_format>
```

### Arguments

-   `input_file`: Path to your input dataset file.
-   `input_format`: The format of your input file. Use `AIL` or `COCO`.
-   `output_file`: Path to save the converted dataset file.
-   `output_format`: The format for your output file. Use `AIL` or `COCO`.

### Examples

**AIL to COCO:**
```bash
python convert_dataset.py dataset.json AIL dataset_coco.json COCO
```

**COCO to AIL:**
```bash
python convert_dataset.py dataset_coco.json COCO dataset_ail.json AIL
```

## Dependencies

- `ail<version>`: AIL library for image processing
