#include "3ds.h"
#include "csvc.h"
#include "CTRPluginFramework.hpp"
#include "TextColors.h"
#include "cheats.hpp"
#include <vector>

namespace CTRPluginFramework
{
	
	//about
	#define MAJOR_VERSION		0
	#define MINOR_VERSION		0
	#define REVISION_VERSION	0
	#define STRINGIFY( x )		#x
	#define TOSTRING( x )		STRINGIFY( x )
	#define STRING_VERSION		"[" TOSTRING( MAJOR_VERSION ) "." TOSTRING( MINOR_VERSION ) "." TOSTRING( REVISION_VERSION ) "]"

	static const std::string About = "RandomHacking\n\n" \
		"バージョン" STRING_VERSION "\n"
	"制作者:Nekotan \n\n";
    // This patch the NFC disabling the touchscreen when scanning an amiibo, which prevents ctrpf to be used
    static void    ToggleTouchscreenForceOn(void)
    {
        static u32 original = 0;
        static u32 *patchAddress = nullptr;

        if (patchAddress && original)
        {
            *patchAddress = original;
            return;
        }

        static const std::vector<u32> pattern =
        {
            0xE59F10C0, 0xE5840004, 0xE5841000, 0xE5DD0000,
            0xE5C40008, 0xE28DD03C, 0xE8BD80F0, 0xE5D51001,
            0xE1D400D4, 0xE3510003, 0x159F0034, 0x1A000003
        };

        Result  res;
        Handle  processHandle;
        s64     textTotalSize = 0;
        s64     startAddress = 0;
        u32 *   found;

        if (R_FAILED(svcOpenProcess(&processHandle, 16)))
            return;

        svcGetProcessInfo(&textTotalSize, processHandle, 0x10002);
        svcGetProcessInfo(&startAddress, processHandle, 0x10005);
        if(R_FAILED(svcMapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x14000000, processHandle, (u32)startAddress, textTotalSize)))
            goto exit;

        found = (u32 *)Utils::Search<u32>(0x14000000, (u32)textTotalSize, pattern);

        if (found != nullptr)
        {
            original = found[13];
            patchAddress = (u32 *)PA_FROM_VA((found + 13));
            found[13] = 0xE1A00000;
        }

        svcUnmapProcessMemoryEx(CUR_PROCESS_HANDLE, 0x14000000, textTotalSize);
exit:
        svcCloseHandle(processHandle);
    }

    // This function is called before main and before the game starts
    // Useful to do code edits safely
    void    PatchProcess(FwkSettings &settings)
    {
        ToggleTouchscreenForceOn();
		// UIの色
		// テキストの色
		//settings.WindowTitleColor = Color(colorUiWindowTitle);
		// タイトルの色
		// settings.MainTextColor = Color(colorUiMainText);
		// 普通のメニューの色
		// settings.MenuSelectedItemColor = Color(colorUiMenuSelectedItem);
		// ActionReplayとかの色
		// settings.MenuUnselectedItemColor = Color(colorUiMenuUnselectedItem);
		// 背景の色
		// settings.BackgroundMainColor = Color(colorUiBackgroundMain);
		// 背景の細線の色
		settings.BackgroundSecondaryColor = Color(ccLightPink2);
		// キーボードの色
		// キーボードの背景の色
		// settings.Keyboard.Background = Color(colorKeyboardBackground);
		// キーの色
		settings.Keyboard.KeyBackground = Color(AionCollarlightBlue);
		// 押された時のキーの背景の色
		settings.Keyboard.KeyBackgroundPressed = Color(ccSkyBlue);
		// Color(colorKeyboardKeyBackgroundPressed); キー数字の色
		settings.Keyboard.KeyText = Color(AionCollaryellow);
		//押された時のキーの数字の色
		settings.Keyboard.KeyTextPressed = Color(ccSkyBlue);
    }

    // This function is called when the process exits
    // Useful to save settings, undo patchs or clean up things
    void    OnProcessExit(void)
    {
        ToggleTouchscreenForceOn();
    }

    void    InitMenu(PluginMenu &menu)
    {
		new MenuEntry(Color(ccLightPink2) << "設定", nullptr, settingUi);
		new MenuEntry(Color(ccLightPink2) << "ランダムチェンジャー", nullptr, Random);
    }

    int     main(void)
    {
		
        PluginMenu *menu = new PluginMenu(Color(ccLightPink2) << "ねこたん　randamize",MAJOR_VERSION, MINOR_VERSION, REVISION_VERSION, About );

        // Synnchronize the menu with frame event
        menu->SynchronizeWithFrame(true);
		OSD::Notify(Color(cccrystal) <<   "Nekotan randamize" << Color::DodgerBlue << "Booting");
        // Init our menu entries & folders
        InitMenu(*menu);

        // Launch menu and mainloop
        menu->Run();

        delete menu;

        // Exit plugin
        return (0);
    }
}
