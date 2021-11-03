package com.qiniu.client.curl;

import android.app.Application;
import android.content.Context;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

class CurlUtils {

    static Context applicationContext() {
        try {
            Application app = getApplicationUsingReflection();
            return app.getApplicationContext();
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    static Application getApplicationUsingReflection() throws Exception {
        return (Application) Class.forName("android.app.ActivityThread")
                .getMethod("currentApplication").invoke(null, (Object[]) null);
    }

    static  String sdkDirectory(){
        String directory = applicationContext().getCacheDir().getAbsolutePath() + "/qiniu/curl";
        File f = new File(directory);
        if (!f.exists() && !f.mkdirs()) {
            directory = null;
        }
        return directory;
    }


    synchronized static String getCAPath(){
        String sdkDir = sdkDirectory();

        File caFile = new File(sdkDir, "cacert.pem");
        String caPath = caFile.getPath();
        if (caFile.exists()){
            return caPath;
        }

        if (copyCAFromAssetsToPath(caPath)){
            return caPath;
        } else {
            return null;
        }
    }

    static boolean copyCAFromAssetsToPath(String path) {
        if (path == null){
            return false;
        }

        byte[] data;
        Context context = applicationContext();
        try {
            //获取输入流
            InputStream mAssets = context.getAssets().open("cacert.pem");

            //获取文件的字节数
            int lenght = mAssets.available();
            //创建byte数组
            data = new byte[lenght];
            //将文件中的数据写入到字节数组中
            mAssets.read(data);
            mAssets.close();
        } catch (IOException e) {
            return false;
        }

        return saveDataToPath(data, path);
    }

    static boolean saveDataToPath(byte[] data, String path) {
        if (path == null) {
            return false;
        }
        boolean ret = true;
        File f = new File(path);
        FileOutputStream fo = null;
        try {
            fo = new FileOutputStream(f);
            fo.write(data);
        } catch (IOException e) {
            ret = false;
        }
        if (fo != null) {
            try {
                fo.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return ret;
    }
}
