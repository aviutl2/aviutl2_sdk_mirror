# AviUtl2 SDK Mirror

AviUtl2 SDKのミラーです。
毎日0時に自動で更新されます。

本家と違い、以下の点が異なります：

- `aviutl2_plugin_sdk.txt`は`README.md`と`CHANGELOG.md`に分割されています。
- `{input2,output2}.h`ファイルは`include/aviutl2_sdk/`に移動されています。
- `{AviReader,AviSaver}.cpp`は`examples/`に移動されています。
- すべてのファイルがUTF-8 LFに変換されています。

更新スクリプトは[./scripts/Rakefile](./scripts/Rakefile)を参照してください。

> [!WARNING]
> これは有志による非公式のミラーです。

以下はAviUtl2 SDKの説明です。

---

# AviUtl ExEdit2 Plugin SDK by ＫＥＮくん

## はじめに

AviUtl ExEdit2のPluginを作成するためのSDKです。  
Pluginを追加することで他のファイル形式の入出力機能を
AviUtl ExEdit2に追加することが出来ます。

## プラグインの種類

- 入力プラグイン (.aui2)\
   他のファイル形式を読み込むプラグインです。

- 出力プラグイン (.auo2)\
   他のファイル形式に出力するプラグインです。

※現状は入力、出力プラグインのみの対応となります。

## サンプルプラグイン

各種サンプルプラグインのソースを付属していますので
作成するときの参考にしてみてください。

- AviReader\
   AVIファイルを読み込むプラグインです。

- AviSaver\
   AVIファイルで出力するプラグインです。

## 使用上の注意

このプログラムはフリーソフトウェアです。  
このプログラムの使用によって何らかの障害が発生した場合でも、
作者は一切の責任を負わないものとします。

## ライセンス

AviUtl ExEdit2 Plugin SDK は MIT License の元で公開されています。  
ライセンスについては付属の license.txt をご覧ください。
