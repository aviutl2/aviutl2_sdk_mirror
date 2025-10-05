//----------------------------------------------------------------------------------
//	サンプルフィルタプラグイン(メディアオブジェクト) for AviUtl ExEdit2
//----------------------------------------------------------------------------------
#include <windows.h>
#include <memory>
#include <algorithm>

#include "filter2.h"

bool func_proc_video(FILTER_PROC_VIDEO* video);
bool func_proc_audio(FILTER_PROC_AUDIO* audio);

//---------------------------------------------------------------------
//	フィルタ設定項目定義
//---------------------------------------------------------------------
auto width = FILTER_ITEM_TRACK(L"横", 100, 1, 1000);
auto height = FILTER_ITEM_TRACK(L"縦", 100, 1, 1000);
auto color = FILTER_ITEM_COLOR(L"色", 0xffffff);
auto frequency = FILTER_ITEM_TRACK(L"周波数", 1000, 1, 24000);
void* items[] = { &width, &height, &color, &frequency, nullptr };

//---------------------------------------------------------------------
//	フィルタプラグイン構造体定義
//---------------------------------------------------------------------
FILTER_PLUGIN_TABLE filter_plugin_table = {
	FILTER_PLUGIN_TABLE::FLAG_VIDEO | FILTER_PLUGIN_TABLE::FLAG_AUDIO | FILTER_PLUGIN_TABLE::FLAG_INPUT, //	フラグ
	L"MediaObject(sample)",							// プラグインの名前
	L"サンプル",									// ラベルの初期値 (nullptrならデフォルトのラベルになります)
	L"Sample MediaObject version 2.00 By ＫＥＮくん",	// プラグインの情報
	items,											// 設定項目の定義 (FILTER_ITEM_XXXポインタを列挙してnull終端したリストへのポインタ)
	func_proc_video,								// 画像フィルタ処理関数へのポインタ (FLAG_VIDEOが有効の時のみ呼ばれます)
	func_proc_audio									// 音声フィルタ処理関数へのポインタ (FLAG_AUDIOが有効の時のみ呼ばれます)
};

//---------------------------------------------------------------------
//	プラグインDLL初期化関数 (未定義なら呼ばれません)
//---------------------------------------------------------------------
EXTERN_C __declspec(dllexport) bool InitializePlugin(DWORD version) { // versionは本体のバージョン番号
	return true;
}

//---------------------------------------------------------------------
//	プラグインDLL解放関数 (未定義なら呼ばれません)
//---------------------------------------------------------------------
EXTERN_C __declspec(dllexport) void UninitializePlugin() {
}

//---------------------------------------------------------------------
//	フィルタ構造体のポインタを渡す関数
//---------------------------------------------------------------------
EXTERN_C __declspec(dllexport) FILTER_PLUGIN_TABLE* GetFilterPluginTable(void) {
	return &filter_plugin_table;
}

//---------------------------------------------------------------------
//	画像フィルタ処理
//---------------------------------------------------------------------
bool func_proc_video(FILTER_PROC_VIDEO* video) {
	auto w = (int)width.value;
	auto h = (int)height.value;
	if (w <= 0 || h <= 0) return false;

	// 指定サイズ、色の四角形の画像データを作成
	auto col = color.value;
	auto buffer = std::make_unique<PIXEL_RGBA[]>(w * h);
	auto p = buffer.get();
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			p->r = col.r;
			p->g = col.g;
			p->b = col.b;
			p->a = 255;
			p++;
		}
	}

	video->set_image_data(buffer.get(), w, h);
	return true;
}

//---------------------------------------------------------------------
//	音声フィルタ処理
//---------------------------------------------------------------------
bool func_proc_audio(FILTER_PROC_AUDIO* audio) {
	auto sample_index = audio->object->sample_index;
	auto sample_num = audio->object->sample_num;
	auto channel_num = audio->object->channel_num;

	// 指定周波数のサイン波の音声データを作成
	auto step = (3.141592653589793 * 2.0) * frequency.value / audio->scene->sample_rate;
	auto buffer = std::make_unique<float[]>(sample_num);
	auto p = buffer.get();
	for (int i = 0; i < sample_num; i++) {
		*p++ = (float)sin(sample_index++ * step);
	}

	for (int i = 0; i < channel_num; i++) {
		audio->set_sample_data(buffer.get(), i);
	}
	return true;
}
