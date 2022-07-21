# qiniu-android-curl-plugin

[![@qiniu on weibo](http://img.shields.io/badge/weibo-%40qiniutek-blue.svg)](http://weibo.com/qiniutek)
[![Software License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](LICENSE.md)
[![Build Status](https://github.com/qiniu/android-sdk/workflows/Run%20Test%20Cases/badge.svg)](https://github.com/qiniu/android-sdk/actions)
[![codecov](https://codecov.io/gh/qiniu/android-sdk/branch/master/graph/badge.svg)](https://codecov.io/gh/qiniu/android-sdk)
[![Latest Stable Version](http://img.shields.io/maven-central/v/com.qiniu/qiniu-android-sdk.svg)](https://search.maven.org/search?q=a:qiniu-android-sdk)

Qiniu Android SDK 插件，提供对 libcurl 集成功能


## 安装

### 运行环境

| Qiniu SDK 版本 | 最低 Android版本   |       依赖库版本           |
|------------ |-----------------|------------------------|
|  7.0.*        |  Android 2.2+     | qiniu-android-sdk v8.5.0|

### 注意
* 推荐使用最新版：8.5.0
* 7.6.2 ~ 8.3.2 AndroidNetwork.getMobileDbm()可以获取手机信号强度，需要如下权限(API>=18时生效)
```
  <uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION"/>
  <uses-permission android:name="android.permission.ACCESS_FINE_LOCATION"/>
```
* 从7.5.0开始增加了DNS预取和缓存策略，减少dns解析错误
* 如果可以明确 区域 的话，最好指定固定区域，这样可以少一步网络请求，少一步出错的可能。
* 如果使用 Android 4.x ，对应 okhttp 版本请调整至 3.12.+

### 直接安装
将 sdk jar 文件 复制到项目中去，[jar包下载地址](http://search.maven.org/#search%7Cga%7C1%7Ccom%2Fqiniu%2Fqiniu-android-curl-plugin) , 下载对应的jar包，以及搜索下载对应的依赖库

[qiniu-android-sdk下载地址](http://search.maven.org/#search%7Cga%7C1%7Ccom%2Fqiniu%2Fqiniu-android-sdk)

### 通过maven
* Android Studio中添加dependencies 或者 在项目中添加maven依赖
```
// 1. 直接导入
implementation 'com.qiniu:qiniu-android-plugin:0.0.+'

// 2. 如果要修改 qiniu-android-sdk 依赖的版本，可采用以下方式（强烈建议使用七牛库依赖的 qiniu-android-sdk 版本）
implementation ('com.qiniu:qiniu-android-plugin:0.0.+'){
    exclude (group: 'com.qiniu', module: 'qiniu-android-sdk')
}
implementation 'com.qiniu:qiniu-android-sdk:8.5.0' // 最低版本 8.5.0
```
* 如果是 eclipse, 也可以直接添加依赖来处理。

## 使用方法
```java
import com.qiniu.client.curl;
import com.qiniu.android.http.ResponseInfo;
import com.qiniu.android.http.metrics.UploadSingleRequestMetrics;
import com.qiniu.android.http.request.IRequestClient;
import com.qiniu.android.http.request.Request;

// 如果想自定义 CA 可设置此选项，此处为 CA 文件的路径
// CA 文件参考：https://curl.se/ca/cacert.pem
// CurlClient client = new CurlClient("CaPath");

CurlClient client = new CurlClient();
client.request(request, new IRequestClient.Options(null, true, null), null, new IRequestClient.RequestClientCompleteHandler() {
    @Override
    public void complete(ResponseInfo responseInfo, UploadSingleRequestMetrics metrics, JSONObject response) {
        // code
    }
});
```

## 测试

``` bash
$ ./gradlew connectedAndroidTest
```

## 常见问题
**1).关于混淆配置:**

混淆处理 对七牛的SDK不需要做特殊混淆处理，混淆时将七牛相关的包都排除就可以了。

在 Android Studio 中，混淆配置在 proguard-rules.pro 里面加上下面几行混淆代码就行：
```
-keep class com.qiniu.**{*;}
-keep class com.qiniu.**{public <init>();}
-ignorewarnings
```
注意：-ignorewarnings这个也是必须加的，如果不加这个编译的时候可能是可以通过的，但是release的时候还是会出现错误。

对于Eeclipse中也是一样的，在proguard-project.txt 文件附件同样的排除代码就可以了
```
-keep class com.qiniu.**{*;}
-keep class com.qiniu.**{public <init>();}
-ignorewarnings
```


## 代码贡献

详情参考[代码提交指南](https://github.com/qiniu/com.qiniu:qiniu-android-plugin/blob/master/CONTRIBUTING.md)。

## 贡献记录

- [所有贡献者](https://github.com/qiniu/android-sdk/contributors)

## 联系我们

- 如果需要帮助，请提交工单（在portal右侧点击咨询和建议提交工单，或者直接向 support@qiniu.com 发送邮件）
- 如果有什么问题，可以到问答社区提问，[问答社区](http://qiniu.segmentfault.com/)
- 更详细的文档，见[官方文档站](http://developer.qiniu.com/)
- 如果发现了bug， 欢迎提交 [issue](https://github.com/qiniu/com.qiniu:qiniu-android-plugin/issues)
- 如果有功能需求，欢迎提交 [issue](https://github.com/qiniu/com.qiniu:qiniu-android-plugin/issues)
- 如果要提交代码，欢迎提交 pull request
- 欢迎关注我们的[微信](http://www.qiniu.com/#weixin) [微博](http://weibo.com/qiniutek)，及时获取动态信息。


## 代码许可

The MIT License (MIT).详情见 [License文件](https://github.com/qiniu/com.qiniu:qiniu-android-plugin/blob/master/LICENSE).