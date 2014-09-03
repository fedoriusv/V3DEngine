#ifndef _V3D_KEY_CODES_H_
#define _V3D_KEY_CODES_H_

namespace v3d
{
	enum EKeyModifierCode 
	{
		eKeyModifier_Uknown = 0x00,
		eKeyModifier_Ctrl   = 0x01,
		eKeyModifier_Alt    = 0x02,
		eKeyModifier_Shift  = 0x04
	};

	enum EKeyCode
	{
		eKeyUknown           = 0x00,  // Default
		eKeyLButton          = 0x01,  // Left mouse button
		eKeyRButton          = 0x02,  // Right mouse button
		eKeyCancel           = 0x03,  // Control-break processing
		eKeyMButton          = 0x04,  // Middle mouse button (three-button mouse)
		eKeyXButton1         = 0x05,  // Windows 2000/XP: X1 mouse button
		eKeyXButton2         = 0x06,  // Windows 2000/XP: X2 mouse button
		eKeyBack             = 0x08,  // BACKSPACE key
		eKeyTab              = 0x09,  // TAB key
		eKeyClear            = 0x0C,  // CLEAR key
		eKeyReturn           = 0x0D,  // ENTER key
		eKeyShift            = 0x10,  // SHIFT key
		eKeyControl          = 0x11,  // CTRL key
		eKeyMenu             = 0x12,  // ALT key
		eKeyPause            = 0x13,  // PAUSE key
		eKeyCapital          = 0x14,  // CAPS LOCK key
		eKeyKana             = 0x15,  // IME Kana mode
		eKeyHanguel          = 0x15,  // IME Hanguel mode (maintained for compatibility use KEY_HANGUL)
		eKeyHangul           = 0x15,  // IME Hangul mode
		eKeyJunja            = 0x17,  // IME Junja mode
		eKeyFinal            = 0x18,  // IME final mode
		eKeyHanja            = 0x19,  // IME Hanja mode
		eKeyKanji            = 0x19,  // IME Kanji mode
		eKeyEscape           = 0x1B,  // ESC key
		eKeyConvert          = 0x1C,  // IME convert
		eKeyNonconvert       = 0x1D,  // IME nonconvert
		eKeyAccept           = 0x1E,  // IME accept
		eKeyModechange       = 0x1F,  // IME mode change request
		eKeySpace            = 0x20,  // SPACEBAR
		eKeyPrior            = 0x21,  // PAGE UP key
		eKeyNext             = 0x22,  // PAGE DOWN key
		eKeyEnd              = 0x23,  // END key
		eKeyHome             = 0x24,  // HOME key
		eKeyLeft             = 0x25,  // LEFT ARROW key
		eKeyUp               = 0x26,  // UP ARROW key
		eKeyRight            = 0x27,  // RIGHT ARROW key
		eKeyDown             = 0x28,  // DOWN ARROW key
		eKeySelect           = 0x29,  // SELECT key
		eKeyPrint            = 0x2A,  // PRINT key
		eKeyExecut           = 0x2B,  // EXECUTE key
		eKeySnapshot         = 0x2C,  // PRINT SCREEN key
		eKeyInsert           = 0x2D,  // INS key
		eKeyDelete           = 0x2E,  // DEL key
		eKeyHelp             = 0x2F,  // HELP key
		eKeyKey_0            = 0x30,  // 0 key
		eKeyKey_1            = 0x31,  // 1 key
		eKeyKey_2            = 0x32,  // 2 key
		eKeyKey_3            = 0x33,  // 3 key
		eKeyKey_4            = 0x34,  // 4 key
		eKeyKey_5            = 0x35,  // 5 key
		eKeyKey_6            = 0x36,  // 6 key
		eKeyKey_7            = 0x37,  // 7 key
		eKeyKey_8            = 0x38,  // 8 key
		eKeyKey_9            = 0x39,  // 9 key
		eKeyKey_A            = 0x41,  // A key
		eKeyKey_B            = 0x42,  // B key
		eKeyKey_C            = 0x43,  // C key
		eKeyKey_D            = 0x44,  // D key
		eKeyKey_E            = 0x45,  // E key
		eKeyKey_F            = 0x46,  // F key
		eKeyKey_G            = 0x47,  // G key
		eKeyKey_H            = 0x48,  // H key
		eKeyKey_I            = 0x49,  // I key
		eKeyKey_J            = 0x4A,  // J key
		eKeyKey_K            = 0x4B,  // K key
		eKeyKey_L            = 0x4C,  // L key
		eKeyKey_M            = 0x4D,  // M key
		eKeyKey_N            = 0x4E,  // N key
		eKeyKey_O            = 0x4F,  // O key
		eKeyKey_P            = 0x50,  // P key
		eKeyKey_Q            = 0x51,  // Q key
		eKeyKey_R            = 0x52,  // R key
		eKeyKey_S            = 0x53,  // S key
		eKeyKey_T            = 0x54,  // T key
		eKeyKey_U            = 0x55,  // U key
		eKeyKey_V            = 0x56,  // V key
		eKeyKey_W            = 0x57,  // W key
		eKeyKey_X            = 0x58,  // X key
		eKeyKey_Y            = 0x59,  // Y key
		eKeyKey_Z            = 0x5A,  // Z key
		eKeyLWin             = 0x5B,  // Left Windows key (Microsoft® Natural® keyboard)
		eKeyRWin             = 0x5C,  // Right Windows key (Natural keyboard)
		eKeyApps             = 0x5D,  // Applications key (Natural keyboard)
		eKeySleep            = 0x5F,  // Computer Sleep key
		eKeyNumpad0          = 0x60,  // Numeric keypad 0 key
		eKeyNumpad1          = 0x61,  // Numeric keypad 1 key
		eKeyNumpad2          = 0x62,  // Numeric keypad 2 key
		eKeyNumpad3          = 0x63,  // Numeric keypad 3 key
		eKeyNumpad4          = 0x64,  // Numeric keypad 4 key
		eKeyNumpad5          = 0x65,  // Numeric keypad 5 key
		eKeyNumpad6          = 0x66,  // Numeric keypad 6 key
		eKeyNumpad7          = 0x67,  // Numeric keypad 7 key
		eKeyNumpad8          = 0x68,  // Numeric keypad 8 key
		eKeyNumpad9          = 0x69,  // Numeric keypad 9 key
		eKeyMultiply         = 0x6A,  // Multiply key
		eKeyAdd              = 0x6B,  // Add key
		eKeySeparator        = 0x6C,  // Separator key
		eKeySubtract         = 0x6D,  // Subtract key
		eKeyDecimal          = 0x6E,  // Decimal key
		eKeyDivide           = 0x6F,  // Divide key
		eKeyF1               = 0x70,  // F1 key
		eKeyF2               = 0x71,  // F2 key
		eKeyF3               = 0x72,  // F3 key
		eKeyF4               = 0x73,  // F4 key
		eKeyF5               = 0x74,  // F5 key
		eKeyF6               = 0x75,  // F6 key
		eKeyF7               = 0x76,  // F7 key
		eKeyF8               = 0x77,  // F8 key
		eKeyF9               = 0x78,  // F9 key
		eKeyF10              = 0x79,  // F10 key
		eKeyF11              = 0x7A,  // F11 key
		eKeyF12              = 0x7B,  // F12 key
		eKeyF13              = 0x7C,  // F13 key
		eKeyF14              = 0x7D,  // F14 key
		eKeyF15              = 0x7E,  // F15 key
		eKeyF16              = 0x7F,  // F16 key
		eKeyF17              = 0x80,  // F17 key
		eKeyF18              = 0x81,  // F18 key
		eKeyF19              = 0x82,  // F19 key
		eKeyF20              = 0x83,  // F20 key
		eKeyF21              = 0x84,  // F21 key
		eKeyF22              = 0x85,  // F22 key
		eKeyF23              = 0x86,  // F23 key
		eKeyF24              = 0x87,  // F24 key
		eKeyNumlock          = 0x90,  // NUM LOCK key
		eKeyScroll           = 0x91,  // SCROLL LOCK key
		eKeyLShift           = 0xA0,  // Left SHIFT key
		eKeyRShift           = 0xA1,  // Right SHIFT key
		eKeyLControl         = 0xA2,  // Left CONTROL key
		eKeyRControl         = 0xA3,  // Right CONTROL key
		eKeyLMenu            = 0xA4,  // Left MENU key
		eKeyRMenu            = 0xA5,  // Right MENU key
		eKeyPlus             = 0xBB,  // Plus Key   (+)
		eKeyComma            = 0xBC,  // Comma Key  (,)
		eKeyMinus            = 0xBD,  // Minus Key  (-)
		eKeyPeriod           = 0xBE,  // Period Key (.)
		eKeyAttn             = 0xF6,  // Attn key
		eKeyCrSel            = 0xF7,  // CrSel key
		eKeyExSel            = 0xF8,  // ExSel key
		eKeyErEof            = 0xF9,  // Erase EOF key
		eKeyPlay             = 0xFA,  // Play key
		eKeyZoom             = 0xFB,  // Zoom key
		eKeyPA1              = 0xFD,  // PA1 key
		eKeyOem_Clear        = 0xFE,  // Clear key
		
		eKey_Codes_Count      = 0xFF   // Count
	};
}

#endif //_V3D_KEY_CODES_H_
