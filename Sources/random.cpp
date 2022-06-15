#include <cstdlib>      // srand,rand
#include "cheats.hpp"
#include <iostream>
#include <random>
#include "3ds.h"
#include "csvc.h"
#include "CTRPluginFramework.hpp"
#include "TextColors.h"
#include "cheats.hpp"
#include <vector>

using std::cout;
using std::endl;

namespace CTRPluginFramework
{
	u32 Mainaddres4 = 0x087D84FC;
	u32 Mainaddres3 = 0x087D8508;
	u32 Mainaddres2 = 0x087D8514;
	u32 Mainaddres1 = 0x087D8520;
	int Randomvalue;	//何体
	u32 value[255];
	int MIN = 0;
	int MAX = 0;
	std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> distr(MIN, MAX);
	
	void settingUi(MenuEntry *entry)
	{
		//何体決めるか指定する
		Keyboard k("ランダムにするための妖怪の数（例4)");
		Randomvalue = k.Open();
		(MessageBox("数が指定されました")());
		//指定された回数入力する
		for(int i = 0;i < Randomvalue,i++;)
		{
			Keyboard k("格納する妖怪ID");
			value[i] = k.Open();
			(MessageBox("設定完了!")());
		}
	}
	
	void Random(MenuEntry *entry)
	{
		MAX = Randomvalue;
		int num = rand() % MAX;
		Process::Write32(Mainaddres1,value[num]);
		num = rand() % MAX;
		Process::Write32(Mainaddres2,value[num]);
		num = rand() % MAX;
		Process::Write32(Mainaddres3,value[num]);
		num = rand() % MAX;
		Process::Write32(Mainaddres4,value[num]);
		(MessageBox("セット完了!")());
	}
}