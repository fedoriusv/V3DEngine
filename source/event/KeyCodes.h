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
		eKeyUknown    ,  // Default
		eKeyLButton   ,  // Left mouse button
		eKeyRButton   ,  // Right mouse button
		eKeyCancel    ,  // Control-break processing
		eKeyMButton   ,  // Middle mouse button (three-button mouse)
		eKeyXButton1  ,  // Windows 2000/XP: X1 mouse button
		eKeyXButton2  ,  // Windows 2000/XP: X2 mouse button
		eKeyBack      ,  // BACKSPACE key
		eKeyTab       ,  // TAB key
		eKeyClear     ,  // CLEAR key
		eKeyReturn    ,  // ENTER key
		eKeyShift     ,  // SHIFT key
		eKeyControl   ,  // CTRL key
		eKeyMenu      ,  // ALT key
		eKeyPause     ,  // PAUSE key
		eKeyCapital   ,  // CAPS LOCK key
		eKeyKana      ,  // IME Kana mode
		eKeyHanguel   ,  // IME Hanguel mode (maintained for compatibility use KEY_HANGUL)
		eKeyHangul    ,  // IME Hangul mode
		eKeyJunja     ,  // IME Junja mode
		eKeyFinal     ,  // IME final mode
		eKeyHanja     ,  // IME Hanja mode
		eKeyKanji     ,  // IME Kanji mode
		eKeyEscape    ,  // ESC key
		eKeyConvert   ,  // IME convert
		eKeyNonconvert,  // IME nonconvert
		eKeyAccept    ,  // IME accept
		eKeyModechange,  // IME mode change request
		eKeySpace     ,  // SPACEBAR
		eKeyPrior     ,  // PAGE UP key
		eKeyNext      ,  // PAGE DOWN key
		eKeyEnd       ,  // END key
		eKeyHome      ,  // HOME key
		eKeyLeft      ,  // LEFT ARROW key
		eKeyUp        ,  // UP ARROW key
		eKeyRight     ,  // RIGHT ARROW key
		eKeyDown      ,  // DOWN ARROW key
		eKeySelect    ,  // SELECT key
		eKeyPrint     ,  // PRINT key
		eKeyExecut    ,  // EXECUTE key
		eKeySnapshot  ,  // PRINT SCREEN key
		eKeyInsert    ,  // INS key
		eKeyDelete    ,  // DEL key
		eKeyHelp      ,  // HELP key
		eKeyKey_0     ,  // 0 key
		eKeyKey_1     ,  // 1 key
		eKeyKey_2     ,  // 2 key
		eKeyKey_3     ,  // 3 key
		eKeyKey_4     ,  // 4 key
		eKeyKey_5     ,  // 5 key
		eKeyKey_6     ,  // 6 key
		eKeyKey_7     ,  // 7 key
		eKeyKey_8     ,  // 8 key
		eKeyKey_9     ,  // 9 key
		eKeyKey_A     ,  // A key
		eKeyKey_B     ,  // B key
		eKeyKey_C     ,  // C key
		eKeyKey_D     ,  // D key
		eKeyKey_E     ,  // E key
		eKeyKey_F     ,  // F key
		eKeyKey_G     ,  // G key
		eKeyKey_H     ,  // H key
		eKeyKey_I     ,  // I key
		eKeyKey_J     ,  // J key
		eKeyKey_K     ,  // K key
		eKeyKey_L     ,  // L key
		eKeyKey_M     ,  // M key
		eKeyKey_N     ,  // N key
		eKeyKey_O     ,  // O key
		eKeyKey_P     ,  // P key
		eKeyKey_Q     ,  // Q key
		eKeyKey_R     ,  // R key
		eKeyKey_S     ,  // S key
		eKeyKey_T     ,  // T key
		eKeyKey_U     ,  // U key
		eKeyKey_V     ,  // V key
		eKeyKey_W     ,  // W key
		eKeyKey_X     ,  // X key
		eKeyKey_Y     ,  // Y key
		eKeyKey_Z     ,  // Z key
		eKeyLWin      ,  // Left Windows key (Microsoft® Natural® keyboard)
		eKeyRWin      ,  // Right Windows key (Natural keyboard)
		eKeyApps      ,  // Applications key (Natural keyboard)
		eKeySleep     ,  // Computer Sleep key
		eKeyNumpad0   ,  // Numeric keypad 0 key
		eKeyNumpad1   ,  // Numeric keypad 1 key
		eKeyNumpad2   ,  // Numeric keypad 2 key
		eKeyNumpad3   ,  // Numeric keypad 3 key
		eKeyNumpad4   ,  // Numeric keypad 4 key
		eKeyNumpad5   ,  // Numeric keypad 5 key
		eKeyNumpad6   ,  // Numeric keypad 6 key
		eKeyNumpad7   ,  // Numeric keypad 7 key
		eKeyNumpad8   ,  // Numeric keypad 8 key
		eKeyNumpad9   ,  // Numeric keypad 9 key
		eKeyMultiply  ,  // Multiply key
		eKeyAdd       ,  // Add key
		eKeySeparator ,  // Separator key
		eKeySubtract  ,  // Subtract key
		eKeyDecimal   ,  // Decimal key
		eKeyDivide    ,  // Divide key
		eKeyF1        ,  // F1 key
		eKeyF2        ,  // F2 key
		eKeyF3        ,  // F3 key
		eKeyF4        ,  // F4 key
		eKeyF5        ,  // F5 key
		eKeyF6        ,  // F6 key
		eKeyF7        ,  // F7 key
		eKeyF8        ,  // F8 key
		eKeyF9        ,  // F9 key
		eKeyF10       ,  // F10 key
		eKeyF11       ,  // F11 key
		eKeyF12       ,  // F12 key
		eKeyF13       ,  // F13 key
		eKeyF14       ,  // F14 key
		eKeyF15       ,  // F15 key
		eKeyF16       ,  // F16 key
		eKeyF17       ,  // F17 key
		eKeyF18       ,  // F18 key
		eKeyF19       ,  // F19 key
		eKeyF20       ,  // F20 key
		eKeyF21       ,  // F21 key
		eKeyF22       ,  // F22 key
		eKeyF23       ,  // F23 key
		eKeyF24       ,  // F24 key
		eKeyNumlock   ,  // NUM LOCK key
		eKeyScroll    ,  // SCROLL LOCK key
		eKeyLShift    ,  // Left SHIFT key
		eKeyRShift    ,  // Right SHIFT key
		eKeyLControl  ,  // Left CONTROL key
		eKeyRControl  ,  // Right CONTROL key
		eKeyLMenu     ,  // Left MENU key
		eKeyRMenu     ,  // Right MENU key
		eKeyPlus      ,  // Plus Key   (+)
		eKeyComma     ,  // Comma Key  (,)
		eKeyMinus     ,  // Minus Key  (-)
		eKeyPeriod    ,  // Period Key (.)
		eKeyAttn      ,  // Attn key
		eKeyCrSel     ,  // CrSel key
		eKeyExSel     ,  // ExSel key
		eKeyErEof     ,  // Erase EOF key
		eKeyPlay      ,  // Play key
		eKeyZoom      ,  // Zoom key
		eKeyPA1       ,  // PA1 key
		eKeyOem_Clear ,  // Clear key
		
		eKey_Codes_Count  = 0xFF   // Count
	};
}

#endif //_V3D_KEY_CODES_H_
