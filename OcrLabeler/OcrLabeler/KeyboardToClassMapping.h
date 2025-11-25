#pragma once

#include "Header.h"

// Note that these characters are not supported by the tool: €, £, ¥, ¢, `
std::unordered_map<AIL_INT, AIL_STRING> UC_SC_KEYCLASSMAP = {
   {M_KEY_NUMPAD0,L"0"},
   {M_KEY_NUMPAD1,L"1"},
   {M_KEY_NUMPAD2,L"2"},
   {M_KEY_NUMPAD3,L"3"},
   {M_KEY_NUMPAD4,L"4"},
   {M_KEY_NUMPAD5,L"5"},
   {M_KEY_NUMPAD6,L"6"},
   {M_KEY_NUMPAD7,L"7"},
   {M_KEY_NUMPAD8,L"8"},
   {M_KEY_NUMPAD9,L"9"},
   {M_KEY_A,L"A"},
   {M_KEY_B,L"B"},
   {M_KEY_C,L"C"},
   {M_KEY_D,L"D"},
   {M_KEY_E,L"E"},
   {M_KEY_F,L"F"},
   {M_KEY_G,L"G"},
   {M_KEY_H,L"H"},
   {M_KEY_I,L"I"},
   {M_KEY_J,L"J"},
   {M_KEY_K,L"K"},
   {M_KEY_L,L"L"},
   {M_KEY_M,L"M"},
   {M_KEY_N,L"N"},
   {M_KEY_O,L"O"},
   {M_KEY_P,L"P"},
   {M_KEY_Q,L"Q"},
   {M_KEY_R,L"R"},
   {M_KEY_S,L"S"},
   {M_KEY_T,L"T"},
   {M_KEY_U,L"U"},
   {M_KEY_V,L"V"},
   {M_KEY_W,L"W"},
   {M_KEY_X,L"X"},
   {M_KEY_Y,L"Y"},
   {M_KEY_Z,L"Z"},
   {M_KEY_0, L")"},  // '0' key with Shift
   {M_KEY_1, L"!"},  // '1' key with Shift
   {M_KEY_2, L"@"},  // '2' key with Shift
   {M_KEY_3, L"#"},  // '3' key with Shift
   {M_KEY_4, L"$"},  // '4' key with Shift
   {M_KEY_5, L"%"},  // '5' key with Shift
   {M_KEY_6, L"^"},  // '6' key with Shift
   {M_KEY_7, L"&"},  // '7' key with Shift
   {M_KEY_8, L"*"},  // '8' key with Shift
   {M_KEY_9, L"("},  // '9' key with Shift
   {0xDE, L"\""},    // Apostrophe key with Shift (")
   {0xBB, L"+"},     // Equals key with Shift
   {0xBA, L":"},     // Semicolon key with Shift
   {0xBC, L"<"},     // Comma key with Shift
   {0xBE, L">"},     // Period key with Shift
   {0xBF, L"?"},     // Slash key with Shift
   {0xBD, L"_"},     // Minus key with Shift
   {0xDB, L"{"},     // Open bracket key with Shift
   {0xDC, L"|"},     // Backslash key with Shift
   {0xDD, L"}"},     // Close bracket key with Shift
   {0xC0, L"~"}      // Backquote key with Shift
   };

std::unordered_map<AIL_INT, AIL_STRING> LC_SC_KEYCLASSMAP = {
   {M_KEY_NUMPAD0,L"0"},
   {M_KEY_NUMPAD1,L"1"},
   {M_KEY_NUMPAD2,L"2"},
   {M_KEY_NUMPAD3,L"3"},
   {M_KEY_NUMPAD4,L"4"},
   {M_KEY_NUMPAD5,L"5"},
   {M_KEY_NUMPAD6,L"6"},
   {M_KEY_NUMPAD7,L"7"},
   {M_KEY_NUMPAD8,L"8"},
   {M_KEY_NUMPAD9,L"9"},
   {M_KEY_A,L"a"},
   {M_KEY_B,L"b"},
   {M_KEY_C,L"c"},
   {M_KEY_D,L"d"},
   {M_KEY_E,L"e"},
   {M_KEY_F,L"f"},
   {M_KEY_G,L"g"},
   {M_KEY_H,L"h"},
   {M_KEY_I,L"i"},
   {M_KEY_J,L"j"},
   {M_KEY_K,L"k"},
   {M_KEY_L,L"l"},
   {M_KEY_M,L"m"},
   {M_KEY_N,L"n"},
   {M_KEY_O,L"o"},
   {M_KEY_P,L"p"},
   {M_KEY_Q,L"q"},
   {M_KEY_R,L"r"},
   {M_KEY_S,L"s"},
   {M_KEY_T,L"t"},
   {M_KEY_U,L"u"},
   {M_KEY_V,L"v"},
   {M_KEY_W,L"w"},
   {M_KEY_X,L"x"},
   {M_KEY_Y,L"y"},
   {M_KEY_Z,L"z"},
   {M_KEY_0, L")"},  // '0' key with Shift
   {M_KEY_1, L"!"},  // '1' key with Shift
   {M_KEY_2, L"@"},  // '2' key with Shift
   {M_KEY_3, L"#"},  // '3' key with Shift
   {M_KEY_4, L"$"},  // '4' key with Shift
   {M_KEY_5, L"%"},  // '5' key with Shift
   {M_KEY_6, L"^"},  // '6' key with Shift
   {M_KEY_7, L"&"},  // '7' key with Shift
   {M_KEY_8, L"*"},  // '8' key with Shift
   {M_KEY_9, L"("},  // '9' key with Shift
   {0xDE, L"\""},    // Apostrophe key with Shift (")
   {0xBB, L"+"},     // Equals key with Shift
   {0xBA, L":"},     // Semicolon key with Shift
   {0xBC, L"<"},     // Comma key with Shift
   {0xBE, L">"},     // Period key with Shift
   {0xBF, L"?"},     // Slash key with Shift
   {0xBD, L"_"},     // Minus key with Shift
   {0xDB, L"{"},     // Open bracket key with Shift
   {0xDC, L"|"},     // Backslash key with Shift
   {0xDD, L"}"},     // Close bracket key with Shift
   {0xC0, L"~"}      // Backquote key with Shift
   };

std::unordered_map<AIL_INT, AIL_STRING> UC_NUM_KEYCLASSMAP = {
   {M_KEY_0,L"0"},{M_KEY_NUMPAD0,L"0"},
   {M_KEY_1,L"1"},{M_KEY_NUMPAD1,L"1"},
   {M_KEY_2,L"2"},{M_KEY_NUMPAD2,L"2"},
   {M_KEY_3,L"3"},{M_KEY_NUMPAD3,L"3"},
   {M_KEY_4,L"4"},{M_KEY_NUMPAD4,L"4"},
   {M_KEY_5,L"5"},{M_KEY_NUMPAD5,L"5"},
   {M_KEY_6,L"6"},{M_KEY_NUMPAD6,L"6"},
   {M_KEY_7,L"7"},{M_KEY_NUMPAD7,L"7"},
   {M_KEY_8,L"8"},{M_KEY_NUMPAD8,L"8"},
   {M_KEY_9,L"9"},{M_KEY_NUMPAD9,L"9"},
   {M_KEY_A,L"A"},
   {M_KEY_B,L"B"},
   {M_KEY_C,L"C"},
   {M_KEY_D,L"D"},
   {M_KEY_E,L"E"},
   {M_KEY_F,L"F"},
   {M_KEY_G,L"G"},
   {M_KEY_H,L"H"},
   {M_KEY_I,L"I"},
   {M_KEY_J,L"J"},
   {M_KEY_K,L"K"},
   {M_KEY_L,L"L"},
   {M_KEY_M,L"M"},
   {M_KEY_N,L"N"},
   {M_KEY_O,L"O"},
   {M_KEY_P,L"P"},
   {M_KEY_Q,L"Q"},
   {M_KEY_R,L"R"},
   {M_KEY_S,L"S"},
   {M_KEY_T,L"T"},
   {M_KEY_U,L"U"},
   {M_KEY_V,L"V"},
   {M_KEY_W,L"W"},
   {M_KEY_X,L"X"},
   {M_KEY_Y,L"Y"},
   {M_KEY_Z,L"Z"},
   {0xDE, L"'"},  // Apostrophe
   {0xBC, L","},  // Comma
   {0xBD, L"-"},  // Minus
   {0xBE, L"."},  // Period
   {0xBF, L"/"},  // Slash
   {0xBA, L";"},  // Semicolon
   {0xBB, L"="},  // Equals
   {0xDB, L"["},  // Open bracket
   {0xDC, L"\\"}, // Backslash
   {0xDD, L"]"}   // Close bracket
   };

std::unordered_map<AIL_INT, AIL_STRING> LC_NUM_KEYCLASSMAP = {
   {M_KEY_0,L"0"},{M_KEY_NUMPAD0,L"0"},
   {M_KEY_1,L"1"},{M_KEY_NUMPAD1,L"1"},
   {M_KEY_2,L"2"},{M_KEY_NUMPAD2,L"2"},
   {M_KEY_3,L"3"},{M_KEY_NUMPAD3,L"3"},
   {M_KEY_4,L"4"},{M_KEY_NUMPAD4,L"4"},
   {M_KEY_5,L"5"},{M_KEY_NUMPAD5,L"5"},
   {M_KEY_6,L"6"},{M_KEY_NUMPAD6,L"6"},
   {M_KEY_7,L"7"},{M_KEY_NUMPAD7,L"7"},
   {M_KEY_8,L"8"},{M_KEY_NUMPAD8,L"8"},
   {M_KEY_9,L"9"},{M_KEY_NUMPAD9,L"9"},
   {M_KEY_A,L"a"},
   {M_KEY_B,L"b"},
   {M_KEY_C,L"c"},
   {M_KEY_D,L"d"},
   {M_KEY_E,L"e"},
   {M_KEY_F,L"f"},
   {M_KEY_G,L"g"},
   {M_KEY_H,L"h"},
   {M_KEY_I,L"i"},
   {M_KEY_J,L"j"},
   {M_KEY_K,L"k"},
   {M_KEY_L,L"l"},
   {M_KEY_M,L"m"},
   {M_KEY_N,L"n"},
   {M_KEY_O,L"o"},
   {M_KEY_P,L"p"},
   {M_KEY_Q,L"q"},
   {M_KEY_R,L"r"},
   {M_KEY_S,L"s"},
   {M_KEY_T,L"t"},
   {M_KEY_U,L"u"},
   {M_KEY_V,L"v"},
   {M_KEY_W,L"w"},
   {M_KEY_X,L"x"},
   {M_KEY_Y,L"y"},
   {M_KEY_Z,L"z"},
   {0xDE, L"'"},  // Apostrophe
   {0xBC, L","},  // Comma
   {0xBD, L"-"},  // Minus
   {0xBE, L"."},  // Period
   {0xBF, L"/"},  // Slash
   {0xBA, L";"},  // Semicolon
   {0xBB, L"="},  // Equals
   {0xDB, L"["},  // Open bracket
   {0xDC, L"\\"}, // Backslash
   {0xDD, L"]"}   // Close bracket
   };

