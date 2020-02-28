#pragma once

#include "../math/Point.h"


enum class EnterCharState {
	Making,
	Finish,
	None
};

namespace Input {
#ifdef _WIN32
	enum class Key : unsigned char {
		A = 'A', B = 'B', C = 'C', D = 'D', E = 'E', F = 'F', G = 'G', H = 'H', I = 'I', J = 'J', K = 'K', L = 'L', M = 'M', N = 'N', O = 'O', P = 'P', Q = 'Q', R = 'R', S = 'S', T = 'T', U = 'U', V = 'V', W = 'W', X = 'X', Y = 'Y', Z = 'Z'
		, One = '1', Two = '2', Three = '3', Four = '4', Five = '5', Six = '6', Seven = '7', Eight = '8', Nine = '9', Zero = '0',
		Numpad1 = VK_NUMPAD1, Numpad2 = VK_NUMPAD2, Numpad3 = VK_NUMPAD3, Numpad4 = VK_NUMPAD4, Numpad5 = VK_NUMPAD5,
		Numpad6 = VK_NUMPAD6, Numpad7 = VK_NUMPAD7, Numpad8 = VK_NUMPAD8, Numpad9 = VK_NUMPAD9, Numpad0 = VK_NUMPAD0,
		NumLock = VK_NUMLOCK, ScrollLock = VK_SCROLL, CapsLock = VK_CAPITAL,
		NumpadAdd = VK_ADD, NumpadSubtract = VK_SUBTRACT, NumpadMultiply = VK_MULTIPLY, NumpadDivide = VK_DIVIDE, Numpad_Dot = VK_DECIMAL,
		Enter = VK_RETURN,//Numpad and Standard
		LShift = VK_LSHIFT, RShift = VK_RSHIFT, Shift = VK_SHIFT,
		Control = VK_CONTROL, LControl = VK_LCONTROL, RControl = VK_RCONTROL,
		Tap = VK_TAB,
		Esc = VK_ESCAPE,
		IMEProcess = VK_PROCESSKEY,
		Apps = VK_APPS,
		SpaceBar = VK_SPACE,
		F1 = VK_F1, F2 = VK_F2, F3 = VK_F3, F4 = VK_F4, F5 = VK_F5, F6 = VK_F6, F7 = VK_F7, F8 = VK_F8, F9 = VK_F9, F10 = VK_F10, F11 = VK_F11, F12 = VK_F12,
		Dot = VK_OEM_PERIOD,
		Comma = VK_OEM_COMMA,
		Plus = VK_OEM_PLUS,
		Minus = VK_OEM_MINUS,
		BackSlash = VK_OEM_5,//\
		Slash = VK_OEM_2,
		LSquareBracket = VK_OEM_4,//[
		RSquareBracket = VK_OEM_6,//]
		SwungDash = VK_OEM_3,//`
		Insert = VK_INSERT,
		Home = VK_HOME,
		PageUp = VK_PRIOR,
		PageDown = VK_NEXT,
		Delete = VK_DELETE,
		End = VK_END,
		Pause = VK_PAUSE,
		Semicolon = VK_OEM_1,//;
		Apostrophe = VK_OEM_7,//'
		Up = VK_UP,
		Down = VK_DOWN,
		Left = VK_LEFT,
		Right = VK_RIGHT,
		BackSpace = VK_BACK,
		Alt = VK_MENU
	};
#elif __ANDROID__
	enum class Key : int {
		Unknown = AKEYCODE_UNKNOWN,

		SoftLeft = AKEYCODE_SOFT_LEFT,
		SoftRight = AKEYCODE_SOFT_RIGHT,

		Home = AKEYCODE_HOME,
		Back = AKEYCODE_BACK,
		Call = AKEYCODE_CALL,
		EndCall = AKEYCODE_CALL,

		Zero = AKEYCODE_0, One = AKEYCODE_1, Two = AKEYCODE_2, Three = AKEYCODE_3, Four = AKEYCODE_4, Five = AKEYCODE_5, Six = AKEYCODE_6, Seven = AKEYCODE_7, Eight = AKEYCODE_8, Nine = AKEYCODE_9,

        Multiply = AKEYCODE_STAR,
        Sharp = AKEYCODE_POUND,

        Up = AKEYCODE_DPAD_UP,  Down = AKEYCODE_DPAD_DOWN,  Left = AKEYCODE_DPAD_LEFT,  Right = AKEYCODE_DPAD_RIGHT, Center = AKEYCODE_DPAD_CENTER, UpLeft = AKEYCODE_DPAD_UP_LEFT, DownLeft = AKEYCODE_DPAD_DOWN_LEFT, UpRight = AKEYCODE_DPAD_UP_RIGHT, DownRight = AKEYCODE_DPAD_DOWN_RIGHT,

        VolumeUp = AKEYCODE_VOLUME_UP,
		VoulmeDown = AKEYCODE_VOLUME_DOWN,

		Power = AKEYCODE_POWER,
		Camera = AKEYCODE_CAMERA,
		Clear = AKEYCODE_CLEAR,

		A = AKEYCODE_A, B = AKEYCODE_B, C = AKEYCODE_C, D = AKEYCODE_D, E = AKEYCODE_E, F = AKEYCODE_F, G = AKEYCODE_G, H = AKEYCODE_H, I = AKEYCODE_I, J = AKEYCODE_J, K = AKEYCODE_K, L = AKEYCODE_L, M = AKEYCODE_M, N = AKEYCODE_N, O = AKEYCODE_O, P = AKEYCODE_P, Q = AKEYCODE_Q, R = AKEYCODE_R, S = AKEYCODE_S, T = AKEYCODE_T, U = AKEYCODE_U, V = AKEYCODE_V, W = AKEYCODE_W, X = AKEYCODE_X, Y = AKEYCODE_Y, Z = AKEYCODE_Z,

		Comma = AKEYCODE_COMMA,
		Dot = AKEYCODE_PERIOD,
		LAlt = AKEYCODE_ALT_LEFT,
		RAlt = AKEYCODE_ALT_RIGHT,
		LShift = AKEYCODE_SHIFT_LEFT,
		RShift = AKEYCODE_SHIFT_RIGHT,
		Tap = AKEYCODE_TAB,
		SpaceBar = AKEYCODE_SPACE,
		Sym = AKEYCODE_SYM,//Used to enter alternate symbols.
		Browser = AKEYCODE_EXPLORER,//Used to launch a browser application.
		Mail = AKEYCODE_ENVELOPE,
		Enter = AKEYCODE_ENTER,
		BackSpace = AKEYCODE_DEL,
		SwungDash = AKEYCODE_GRAVE,//`
		Minus = AKEYCODE_MINUS,
		Equal = AKEYCODE_EQUALS,
		LSquareBracket = AKEYCODE_LEFT_BRACKET,//[
		RSquareBracket = AKEYCODE_RIGHT_BRACKET,//]
		BackSlash = AKEYCODE_BACKSLASH,//\
		Semicolon = AKEYCODE_SEMICOLON,//;
		Apostrophe = AKEYCODE_APOSTROPHE,//'
		Slash = AKEYCODE_SLASH,// /
		At = AKEYCODE_AT,//@
		Num = AKEYCODE_NUM,
		HeadsetHook = AKEYCODE_HEADSETHOOK,
		Focus = AKEYCODE_FOCUS,
		Plus = AKEYCODE_PLUS,
		Menu = AKEYCODE_MENU,
		Notification = AKEYCODE_NOTIFICATION,
		Search = AKEYCODE_SEARCH,

		Play = AKEYCODE_MEDIA_PLAY,
		Pause = AKEYCODE_MEDIA_PAUSE,
		PlayPause = AKEYCODE_MEDIA_PLAY_PAUSE,
		Stop = AKEYCODE_MEDIA_STOP,
		Next = AKEYCODE_MEDIA_NEXT,
		Prev = AKEYCODE_MEDIA_PREVIOUS,
		Rewind = AKEYCODE_MEDIA_REWIND,
		FastForward = AKEYCODE_MEDIA_FAST_FORWARD,
		MediaClose = AKEYCODE_MEDIA_CLOSE,
		MediaEject = AKEYCODE_MEDIA_EJECT,
		Record = AKEYCODE_MEDIA_RECORD,
		AudioTrack = AKEYCODE_MEDIA_AUDIO_TRACK,
		MediaTopMenu = AKEYCODE_MEDIA_TOP_MENU,
		SkipForward = AKEYCODE_MEDIA_SKIP_FORWARD,
		SkipBackward = AKEYCODE_MEDIA_SKIP_BACKWARD,
		StepForward = AKEYCODE_MEDIA_STEP_FORWARD,
		StepBackward = AKEYCODE_MEDIA_STEP_BACKWARD,

		Mute = AKEYCODE_MUTE,
		PageUp = AKEYCODE_PAGE_UP,
		PageDown = AKEYCODE_PAGE_DOWN,
		PictureSymbols = AKEYCODE_PICTSYMBOLS,
		SwitchCharset = AKEYCODE_SWITCH_CHARSET,

		ButtonA = AKEYCODE_BUTTON_A,
		ButtonB = AKEYCODE_BUTTON_B,
		ButtonC = AKEYCODE_BUTTON_C,
		ButtonX = AKEYCODE_BUTTON_X,
		ButtonY = AKEYCODE_BUTTON_Y,
		ButtonZ = AKEYCODE_BUTTON_Z,
		ButtonL1 = AKEYCODE_BUTTON_L1,
		ButtonR1 = AKEYCODE_BUTTON_R1,
		ButtonL2 = AKEYCODE_BUTTON_L2,
		ButtonR2 = AKEYCODE_BUTTON_R2,
		ButtonLT = AKEYCODE_BUTTON_THUMBL,
		ButtonRT = AKEYCODE_BUTTON_THUMBR,
		ButtonStart = AKEYCODE_BUTTON_START,
		ButtonSelect = AKEYCODE_BUTTON_SELECT,
		ButtonMode = AKEYCODE_BUTTON_MODE,

		Esc = AKEYCODE_ESCAPE,
		ForwardDel = AKEYCODE_FORWARD_DEL,
		LCtrl = AKEYCODE_CTRL_LEFT,
		RCtrl = AKEYCODE_CTRL_RIGHT,
		CapsLock = AKEYCODE_CAPS_LOCK,
		ScrollLock = AKEYCODE_SCROLL_LOCK,
		MetaLeft = AKEYCODE_META_LEFT,
		MetaRight = AKEYCODE_META_RIGHT,
		Func = AKEYCODE_FUNCTION,
		SysReq = AKEYCODE_SYSRQ,
		Break = AKEYCODE_BREAK,
		MoveHome = AKEYCODE_MOVE_HOME,
		MoveEnd = AKEYCODE_MOVE_END,
		Insert = AKEYCODE_INSERT,
		Foward = AKEYCODE_FORWARD,
		F1 = AKEYCODE_F1, F2 = AKEYCODE_F2, F3 = AKEYCODE_F3, F4 = AKEYCODE_F4, F5 = AKEYCODE_F5, F6 = AKEYCODE_F6, F7 = AKEYCODE_F7, F8 = AKEYCODE_F8, F9 = AKEYCODE_F9, F10 = AKEYCODE_F10, F11 = AKEYCODE_F11, F12 = AKEYCODE_F12,
		NumLock = AKEYCODE_NUM_LOCK,
		Numpad0 = AKEYCODE_NUMPAD_0, Numpad1 = AKEYCODE_NUMPAD_1, Numpad2 = AKEYCODE_NUMPAD_2, Numpad3 = AKEYCODE_NUMPAD_3, Numpad4 = AKEYCODE_NUMPAD_4, Numpad5 = AKEYCODE_NUMPAD_5, Numpad6 = AKEYCODE_NUMPAD_6, Numpad7 = AKEYCODE_NUMPAD_7, Numpad8 = AKEYCODE_NUMPAD_8, Numpad9 = AKEYCODE_NUMPAD_9,
		NumpadDivide = AKEYCODE_NUMPAD_DIVIDE,
		NumpadMultiply = AKEYCODE_NUMPAD_MULTIPLY,
		NumpadSubtract = AKEYCODE_NUMPAD_SUBTRACT,
		NumpadAdd = AKEYCODE_NUMPAD_ADD,
		NumpadDot = AKEYCODE_NUMPAD_DOT,
		NumpadComma = AKEYCODE_NUMPAD_COMMA,
		NumpadEnter = AKEYCODE_NUMPAD_ENTER,
		NumpadEqual = AKEYCODE_NUMPAD_EQUALS,
		NumpadLParen = AKEYCODE_NUMPAD_LEFT_PAREN,
		NumpadRParen = AKEYCODE_NUMPAD_RIGHT_PAREN,
		VolumeMute = AKEYCODE_VOLUME_MUTE,
		ChannelUp = AKEYCODE_CHANNEL_UP,
		ChannelDown = AKEYCODE_CHANNEL_DOWN,
		ZoomIn = AKEYCODE_ZOOM_IN,
		ZoomOut = AKEYCODE_ZOOM_OUT,
		TV = AKEYCODE_TV,
		Window = AKEYCODE_WINDOW,
		Guide = AKEYCODE_GUIDE,
		DVR = AKEYCODE_DVR,
		Bookmark = AKEYCODE_BOOKMARK,
		Captions = AKEYCODE_CAPTIONS,
		Settings = AKEYCODE_SETTINGS,

		SetTopBoxPower = AKEYCODE_STB_POWER,
		SetTopBoxInput = AKEYCODE_STB_INPUT,
		AVRPower = AKEYCODE_AVR_POWER,
		AVRInput = AKEYCODE_AVR_INPUT,

		AppSwitch = AKEYCODE_APP_SWITCH,
		Button1 = AKEYCODE_BUTTON_1, Button2 = AKEYCODE_BUTTON_2, Button3 = AKEYCODE_BUTTON_3, Button4 = AKEYCODE_BUTTON_4, Button5 = AKEYCODE_BUTTON_5, Button6 = AKEYCODE_BUTTON_6, Button7 = AKEYCODE_BUTTON_7, Button8 = AKEYCODE_BUTTON_8, Button9 = AKEYCODE_BUTTON_9 , Button10 = AKEYCODE_BUTTON_10, Button11 = AKEYCODE_BUTTON_11, Button12 = AKEYCODE_BUTTON_12, Button13 = AKEYCODE_BUTTON_13, Button14 = AKEYCODE_BUTTON_14, Button15 = AKEYCODE_BUTTON_15, Button16 = AKEYCODE_BUTTON_16,
		LanguageSwitch = AKEYCODE_LANGUAGE_SWITCH,
		MannerMode = AKEYCODE_MANNER_MODE,
		Mode3D = AKEYCODE_3D_MODE,
		Contacts = AKEYCODE_CONTACTS,
		Calendar = AKEYCODE_CALENDAR,
		Music = AKEYCODE_MUSIC,
		Calc = AKEYCODE_CALCULATOR,

		JapaneseZenkakuHankaku = AKEYCODE_ZENKAKU_HANKAKU,
		JapaneseEisu = AKEYCODE_EISU,
		JapaneseMuhenkan = AKEYCODE_MUHENKAN,
		JapaneseHenkan = AKEYCODE_HENKAN,
		JapaneseKatakanaHiragana = AKEYCODE_KATAKANA_HIRAGANA,
		JapaneseYen = AKEYCODE_YEN,
		JapaneseRo = AKEYCODE_RO,
		JapaneseKana = AKEYCODE_KANA,

		Assist = AKEYCODE_ASSIST,
		VoiceAssist = AKEYCODE_VOICE_ASSIST,

		BrightnessDown = AKEYCODE_BRIGHTNESS_DOWN,
		BrightnessUp = AKEYCODE_BRIGHTNESS_UP,
		Sleep = AKEYCODE_SLEEP,
		WakeUp = AKEYCODE_WAKEUP,
		Pairing = AKEYCODE_PAIRING,

		Eleven = AKEYCODE_11,
		Twelve = AKEYCODE_12,

		LastChannel = AKEYCODE_LAST_CHANNEL,

		TVPower = AKEYCODE_TV_POWER,
		TVInput = AKEYCODE_TV_INPUT,
		TVRed = AKEYCODE_PROG_RED,
		TVGreen = AKEYCODE_PROG_GREEN,
		TVYellow = AKEYCODE_PROG_YELLOW,
		TVBlue = AKEYCODE_PROG_BLUE,
		TVDataService = AKEYCODE_TV_DATA_SERVICE,
		ToggleTVRadioService = AKEYCODE_TV_RADIO_SERVICE,
		TVTeletext = AKEYCODE_TV_TELETEXT,
		TVNumberEntry = AKEYCODE_TV_NUMBER_ENTRY,
		TVAnologTerrestrial = AKEYCODE_TV_TERRESTRIAL_ANALOG,
		TVDigitalTerrestrial = AKEYCODE_TV_TERRESTRIAL_DIGITAL,
		TVSatellite = AKEYCODE_TV_SATELLITE,
		TVSatelliteBS = AKEYCODE_TV_SATELLITE_BS,
		TVSatelliteCS = AKEYCODE_TV_SATELLITE_CS,
		TVToggleSatelliteBSOrCS = AKEYCODE_TV_SATELLITE_SERVICE,
		TVNetwork = AKEYCODE_TV_NETWORK,
		TVToggleAntennaOrCable = AKEYCODE_TV_ANTENNA_CABLE,
		TVHDMI1 = AKEYCODE_TV_INPUT_HDMI_1,
		TVHDMI2 = AKEYCODE_TV_INPUT_HDMI_2,
		TVHDMI3 = AKEYCODE_TV_INPUT_HDMI_3,
		TVHDMI4 = AKEYCODE_TV_INPUT_HDMI_4,
		TVComposite1 = AKEYCODE_TV_INPUT_COMPOSITE_1,
		TVComposite2 = AKEYCODE_TV_INPUT_COMPOSITE_2,
		TVComponent1 = AKEYCODE_TV_INPUT_COMPONENT_1,
		TVComponent2 = AKEYCODE_TV_INPUT_COMPONENT_2,
		TVVGA = AKEYCODE_TV_INPUT_VGA_1,
		TVToggleAudioDescription = AKEYCODE_TV_AUDIO_DESCRIPTION,
		TVAudioDescriptionMixUp = AKEYCODE_TV_AUDIO_DESCRIPTION_MIX_UP,
		TVAudioDescriptionMixDown = AKEYCODE_TV_AUDIO_DESCRIPTION_MIX_DOWN,
		TVZoomMode = AKEYCODE_TV_ZOOM_MODE,
		TVContentsMenu = AKEYCODE_TV_CONTENTS_MENU,
		TVMediaContextMenu = AKEYCODE_TV_MEDIA_CONTEXT_MENU,
		TVTimerProgramming = AKEYCODE_TV_TIMER_PROGRAMMING,

		Help = AKEYCODE_HELP,

		NavigatePrev = AKEYCODE_NAVIGATE_PREVIOUS,
		NavigateNext = AKEYCODE_NAVIGATE_NEXT,
		NavigateIn = AKEYCODE_NAVIGATE_IN,
		NavigateOut = AKEYCODE_NAVIGATE_OUT,

		StemPrimary = AKEYCODE_STEM_PRIMARY,
		Stem1 = AKEYCODE_STEM_1,
		Stem2 = AKEYCODE_STEM_2,
		Stem3 = AKEYCODE_STEM_3,

		SoftSleep = AKEYCODE_SOFT_SLEEP,

		Cut = AKEYCODE_CUT,
		Copy = AKEYCODE_COPY,
		Paste = AKEYCODE_PASTE,

		FingerprintNavigationUp = AKEYCODE_SYSTEM_NAVIGATION_UP,
		FingerprintNavigationDown = AKEYCODE_SYSTEM_NAVIGATION_DOWN,
		FingerprintNavigationLeft = AKEYCODE_SYSTEM_NAVIGATION_LEFT,
		FingerprintNavigationRight = AKEYCODE_SYSTEM_NAVIGATION_RIGHT,

		AllApps = AKEYCODE_ALL_APPS,
		Refresh = AKEYCODE_REFRESH,

		ThumbsUp = AKEYCODE_THUMBS_UP,
		ThumbsDown = AKEYCODE_THUMBS_DOWN,

		ProfileSwitch = AKEYCODE_PROFILE_SWITCH
	};
#endif

	PointF GetMousePosScreen();
	PointF GetMousePos();
	bool IsLMouseClicked();
	bool IsLMouseClick();
	bool IsLMouseClicking();
	bool IsRMouseClicked();
	bool IsRMouseClick();
	bool IsRMouseClicking();
	bool IsWheelClicked();
	bool IsWheelClick();
	bool IsWheelClicking();

	bool IsMouseOut();

#ifdef __ANDROID__
	bool IsPointerClick(unsigned _index=0);
	bool IsPointerClicking(unsigned _index = 0);
	bool IsPointerClicked(unsigned _index = 0);

	PointF GetPointPos(unsigned _index = 0);
    PointF GetPointPosScreen(unsigned _index = 0);
#endif

    void IMEStart();
	std::wstring& GetChars();
	EnterCharState GetEnterCharState();
	unsigned GetPrevCharsLen();
	void IMEFinish();


	bool IsKeyDown(Key _keyCode);
	bool IsKeyUp(Key _keyCode);
	bool IsKeyPress(Key _keyCode);
	bool IsKeyPressing(Key _keyCode);
	bool IsKeyPressed(Key _keyCode);

	
	int GetWheelScrolling();
	void ShowCursor(bool _show);
	void ShowKeyboard(bool _show, std::wstring _text);//모바일 전용
};

