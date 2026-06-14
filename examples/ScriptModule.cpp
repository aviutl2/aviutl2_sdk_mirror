//----------------------------------------------------------------------------------
//	サンプルスクリプトモジュールプラグイン for AviUtl ExEdit2
//----------------------------------------------------------------------------------
#include <windows.h>
#include <algorithm>

#include "module2.h"
#include "logger2.h"
#include "filter2.h" // PIXEL_RGBA定義用 

LOG_HANDLE* logger;

//---------------------------------------------------------------------
//	ログ出力機能初期化関数 (未定義なら呼ばれません)
//---------------------------------------------------------------------
EXTERN_C __declspec(dllexport) void InitializeLogger(LOG_HANDLE* handle) {
	logger = handle;
}

//---------------------------------------------------------------------
//	プラグインDLL初期化関数 (未定義なら呼ばれません)
//---------------------------------------------------------------------
EXTERN_C __declspec(dllexport) bool InitializePlugin(DWORD version) {
	return true;
}

//---------------------------------------------------------------------
//	プラグインDLL解放関数 (未定義なら呼ばれません)
//---------------------------------------------------------------------
EXTERN_C __declspec(dllexport) void UninitializePlugin() {
}

//---------------------------------------------------------------------
//	合計を計算するサンプル関数
//---------------------------------------------------------------------
void sum(SCRIPT_MODULE_PARAM* param) {
	// 引数の合計を計算
	double total = 0.0;
	auto num = param->get_param_num();
	for (int i = 0; i < num; i++) {
		total += param->get_param_double(i);
	}
	param->push_result_double(total);
}

//---------------------------------------------------------------------
//	明るさを調整するサンプル関数
//---------------------------------------------------------------------
void luminance(SCRIPT_MODULE_PARAM* param) {
	// 引数を取得
	auto n = param->get_param_num();
	if (n != 4) {
		param->set_error(u8"引数の数が正しくありません");
		return;
	}
	auto p = (PIXEL_RGBA*)param->get_param_data(0);
	auto w = param->get_param_int(1);
	auto h = param->get_param_int(2);
	auto v = param->get_param_double(3);
	if (!p || w <= 0 || h <= 0) {
		param->set_error(u8"引数の値が正しくありません");
		return;
	}

	// 明るさを調整
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			p->r = (unsigned char)std::clamp(p->r * v, 0.0, 255.0);
			p->g = (unsigned char)std::clamp(p->g * v, 0.0, 255.0);
			p->b = (unsigned char)std::clamp(p->b * v, 0.0, 255.0);
			p++;
		}
	}
}

//---------------------------------------------------------------------
//	メタテーブルを返却するサンプル関数
//---------------------------------------------------------------------
void meta_table_func(SCRIPT_MODULE_PARAM* param) {
	logger->log(logger, L"call func");
}

void meta_table_getter(SCRIPT_MODULE_PARAM* param) {
	if (param->get_param_num() != 2) return;
	auto key = param->get_param_string(1); // キー名がindex=1に入る
	if (strcmp(key, "func") == 0) {
		param->push_result_function(meta_table_func, nullptr); // "func"の場合は関数を返却
	} else {
		param->push_result_int((int)strlen(key)); // キー名の長さを返却
	}
}

void meta_table_setter(SCRIPT_MODULE_PARAM* param) {
	if (param->get_param_num() != 3) return;
	auto key = param->get_param_string(1); // キー名がindex=1に入る
	auto value = param->get_param_string(2); // 値がindex=2に入る
	WCHAR buf[256];
	MultiByteToWideChar(CP_UTF8, 0, key, -1, buf, 256);
	logger->log(logger, buf);
	MultiByteToWideChar(CP_UTF8, 0, value, -1, buf, 256);
	logger->log(logger, buf);
}

void meta_table(SCRIPT_MODULE_PARAM* param) {
	// メタテーブルを返却
	param->push_result_meta_table(meta_table_getter, meta_table_setter, nullptr);
}

//---------------------------------------------------------------------
//	スクリプトモジュール関数リスト定義
//---------------------------------------------------------------------
SCRIPT_MODULE_FUNCTION functions[] = {
	{ L"sum", sum },
	{ L"luminance", luminance },
	{ L"new", meta_table },
	{ nullptr }
};

//---------------------------------------------------------------------
//	スクリプトモジュール構造体定義
//---------------------------------------------------------------------
SCRIPT_MODULE_TABLE script_module_table = {
	L"Sample ScriptModule version 2.00 By ＫＥＮくん",	// モジュールの情報
	functions
};

//---------------------------------------------------------------------
//	スクリプトモジュール構造体のポインタを渡す関数
//---------------------------------------------------------------------
EXTERN_C __declspec(dllexport) SCRIPT_MODULE_TABLE* GetScriptModuleTable(void) {
	return &script_module_table;
}
