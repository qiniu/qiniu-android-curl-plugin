package com.qiniu.client.curl;

import android.app.Application;
import android.content.Context;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.cert.Certificate;
import java.security.cert.X509Certificate;

public class CurlUtils {

    private static String customizedCaPath = null;

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
        Context context = applicationContext();
        if (context == null) {
            return null;
        }

        File cacheDir = context.getCacheDir();
        if (cacheDir == null) {
            return null;
        }

        String directory = cacheDir.getAbsolutePath() + "/qiniu/curl";
        File f = new File(directory);
        if (!f.exists() && !f.mkdirs()) {
            directory = null;
        }
        return directory;
    }

    public static synchronized String getCAPath(){
        if (customizedCaPath != null) {
            File caFile = new File(customizedCaPath);
            if (caFile.exists()){
                return customizedCaPath;
            }
        }

        return getDefaultCAPath();
    }

    public static synchronized void setCaPath(String caPath) {
        customizedCaPath = caPath;
    }

    synchronized static String getDefaultCAPath(){
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
            e.printStackTrace();
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
