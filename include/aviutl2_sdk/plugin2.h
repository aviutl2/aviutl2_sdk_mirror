//----------------------------------------------------------------------------------
//	汎用プラグイン ヘッダーファイル for AviUtl ExEdit2
//	By ＫＥＮくん
//----------------------------------------------------------------------------------
struct INPUT_PLUGIN_TABLE;
struct OUTPUT_PLUGIN_TABLE;
struct FILTER_PLUGIN_TABLE;
struct SCRIPT_MODULE_TABLE;

// オブジェクトハンドル
typedef void* OBJECT_HANDLE;

// オブジェクトフレーム情報構造体
// オブジェクトフレーム情報ではフレーム番号、レイヤー番号が0からの番号になります ※UI表示と異なります
struct OBJECT_FRAME_INFO {
	int start;	// 開始フレーム番号
	int end;	// 終了フレーム番号
};

//----------------------------------------------------------------------------------

// 編集情報構造体
// 編集情報ではフレーム番号、レイヤー番号が0からの番号になります ※UI表示と異なります
struct EDIT_INFO {
	int width, height;	// シーンの解像度
	int rate, scale;	// シーンのフレームレート
	int sample_rate;	// シーンのサンプリングレート
	int frame;			// 現在のカーソルのフレーム番号
	int layer;			// 現在のレイヤーの表示開始番号
	int frame_max;		// オブジェクトが存在する最大のフレーム番号
	int layer_max;		// オブジェクトが存在する最大のレイヤー番号
};

// 編集セクション構造体
// 編集セクションではフレーム番号、レイヤー番号が0からの番号になります ※UI表示と異なります
struct EDIT_SECTION {
	// 編集情報
	EDIT_INFO* info;

	// 指定の位置にオブジェクトエイリアスを作成します
	// alias	: オブジェクトエイリアスデータ(UTF-8)へのポインタ
	//			  オブジェクトエイリアスファイルと同じフォーマットになります
	// layer	: 作成するレイヤー番号
	// frame	: 作成するフレーム番号
	// length	: オブジェクトのフレーム数 ※エイリアスデータにフレーム情報が無い場合に利用します
	// 戻り値	: 作成したオブジェクトのハンドル (失敗した場合はnullptrを返却)
	//			  既に存在するオブジェクトに重なったり、エイリアスデータが不正な場合に失敗します
	OBJECT_HANDLE (*create_object_from_alias)(LPCSTR alias, int layer, int frame, int length);

	// 指定のフレーム番号以降にあるオブジェクトを検索します
	// layer	: 検索対象のレイヤー番号
	// frame	: 検索を開始するフレーム番号
	// 戻り値	: 検索したオブジェクトのハンドル (見つからない場合はnullptrを返却)
	OBJECT_HANDLE (*find_object)(int layer, int frame);

	// オブジェクトのフレーム情報を取得します
	// object	: オブジェクトのハンドル
	// 戻り値	: オブジェクトフレーム情報
	OBJECT_FRAME_INFO (*get_object_frame_info)(OBJECT_HANDLE object);

};

// 編集ハンドル構造体
struct EDIT_HANDLE {
	// プロジェクトデータの編集をする為のコールバック関数(func_proc_edit)を呼び出します
	// 編集情報を排他制御する為にコールバック関数内で編集処理をする形になります
	// コールバック関数内で編集したオブジェクトは纏めてUndoに登録されます
	// コールバック関数はメインスレッドから呼ばれます
	// func_proc_edit	: 編集処理のコールバック関数
	// 戻り値			: trueなら成功
	//					  編集が出来ない場合(出力中等)に失敗します
	bool (*call_edit_section)(void (*func_proc_edit)(EDIT_SECTION* edit));

};

//----------------------------------------------------------------------------------

// ホストアプリケーション構造体
struct HOST_APP_TABLE {
	// プラグインの情報を設定する
	// information	: プラグインの情報
	void (*set_plugin_information)(LPCWSTR information);

	// 入力プラグインを登録する
	// input_plugin_table	: 入力プラグイン構造体
	void (*register_input_plugin)(INPUT_PLUGIN_TABLE* input_plugin_table);

	// 出力プラグインを登録する
	// output_plugin_table	: 出力プラグイン構造体
	void (*register_output_plugin)(OUTPUT_PLUGIN_TABLE* output_plugin_table);

	// フィルタプラグインを登録する
	// filter_plugin_table	: フィルタプラグイン構造体
	void (*register_filter_plugin)(FILTER_PLUGIN_TABLE* filter_plugin_table);

	// スクリプトモジュールを登録する
	// script_module_table	: スクリプトモジュール構造体
	void (*register_script_module)(SCRIPT_MODULE_TABLE* script_module_table);

	// インポートメニューを登録する
	// name				: インポートメニューの名称
	// func_proc_import	: インポートメニュー選択時のコールバック関数
	void (*register_import_menu)(LPCWSTR name, void (*func_proc_import)(EDIT_SECTION* edit));

	// エクスポートメニューを登録する
	// name				: エクスポートメニューの名称
	// func_proc_export	: エクスポートメニュー選択時のコールバック関数
	void (*register_export_menu)(LPCWSTR name, void (*func_proc_export)(EDIT_SECTION* edit));

	// ウィンドウクライアントを登録する
	// name		: ウィンドウの名称
	// hwnd		: ウィンドウハンドル
	// ウィンドウにはWS_CHILDが追加され親ウィンドウが設定されます ※WS_POPUPは削除されます
	void (*register_window_client)(LPCWSTR name, HWND hwnd);

	// プロジェクトデータ編集用のハンドルを取得します
	// 戻り値	: 編集ハンドル
	EDIT_HANDLE* (*create_edit_handle)();

};
