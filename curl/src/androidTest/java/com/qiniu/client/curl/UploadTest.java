package com.qiniu.client.curl;

import static org.junit.Assert.assertTrue;

import android.util.Log;

import com.qiniu.android.common.FixedZone;
import com.qiniu.android.http.ResponseInfo;
import com.qiniu.android.http.dns.Dns;
import com.qiniu.android.http.dns.IDnsNetworkAddress;
import com.qiniu.android.storage.Configuration;
import com.qiniu.android.storage.GlobalConfiguration;
import com.qiniu.android.storage.UpCancellationSignal;
import com.qiniu.android.storage.UpCompletionHandler;
import com.qiniu.android.storage.UpProgressHandler;
import com.qiniu.android.storage.UploadManager;
import com.qiniu.android.storage.UploadOptions;
import com.qiniu.android.utils.Etag;
import com.qiniu.android.utils.LogUtil;


import org.json.JSONObject;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;

import java.io.File;
import java.io.IOException;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class UploadTest extends BaseTest {

    private static final String TestHost0 = "upload-z0.qbox.me";
    private static final String TestHost1 = "up-z0.qbox.me";

    private UploadCompleteInfo info = null;

    @Before
    public void setup() {
        LogUtil.setLogLevel(Log.VERBOSE);
        System.out.println("\n=== test before");
    }

    @Test
    public void formUpload() throws Exception {
        String key = "form_2M.zip";
        File file = TempFile.createFile(1024 * 2, key);

        GlobalConfiguration.getInstance().dns = new Dns() {
            @Override
            public List<IDnsNetworkAddress> lookup(String hostname) throws UnknownHostException {
                if (hostname.equals(TestHost0) || hostname.equals(TestHost1)) {
                    List<IDnsNetworkAddress> list = new ArrayList<>();
                    list.add(new HostAddress(hostname, "111.1.36.180"));
                    list.add(new HostAddress(hostname, "218.98.28.87"));
                    list.add(new HostAddress(hostname, "112.13.172.42"));
                    return list;
                } else {
                    return null;
                }
            }
        };

        UploadOptions options = new UploadOptions(null, null, false, new UpProgressHandler() {
            @Override
            public void progress(String key, double percent) {
                System.out.println("=== curl progress key:" + key + " percent:" + percent);
            }
        }, null);

        Configuration config = new Configuration.Builder()
                .chunkSize(1024 * 1024)
                .putThreshold(4 * 1024 * 1024)
                .resumeUploadVersion(Configuration.RESUME_UPLOAD_VERSION_V1)
                .useConcurrentResumeUpload(false)
                .requestClient(new CurlClient())
                .zone(new FixedZone(new String[]{TestHost0, TestHost1}))
                .build();
        UploadManager manager = new UploadManager(config);
        manager.put(file, key, TestConfig.token_z0, new UpCompletionHandler() {
            @Override
            public void complete(String key, ResponseInfo responseInfo, JSONObject response) {
                info = new UploadCompleteInfo();
                info.key = key;
                info.responseInfo = responseInfo;
                info.response = response;
            }
        }, options);

        wait(new WaitConditional() {
            @Override
            public boolean shouldWait() {
                return info == null;
            }
        }, 10 * 60);

        assertTrue("=== responseInfo was null", info.responseInfo != null);
        assertTrue("=== responseInfo:" + info, info.responseInfo.isOK());
        assertTrue("=== upload key  was" + info.key, key.equals(info.key));
        assertTrue("=== response was null", info.response != null);
        String responseHash = info.response.getString("hash");
        String fileHash = Etag.file(file);
        assertTrue("=== file hash:" + responseHash, fileHash.equals(responseHash));
    }

    @Test
    public void testResumeV1Upload() throws Exception  {
        String key = "resume_v1_5M.zip";
        File file = TempFile.createFile(1024 * 5, key);

        GlobalConfiguration.getInstance().dns = new Dns() {
            @Override
            public List<IDnsNetworkAddress> lookup(String hostname) throws UnknownHostException {
                if (hostname.equals(TestHost0) || hostname.equals(TestHost1)) {
                    List<IDnsNetworkAddress> list = new ArrayList<>();
                    list.add(new HostAddress(hostname, "111.1.36.180"));
                    list.add(new HostAddress(hostname, "218.98.28.87"));
                    list.add(new HostAddress(hostname, "112.13.172.42"));
                    return list;
                } else {
                    return null;
                }
            }
        };

        UploadOptions options = new UploadOptions(null, null, false, new UpProgressHandler() {
            @Override
            public void progress(String key, double percent) {
                System.out.println("=== curl progress key:" + key + " percent:" + percent);
            }
        }, null);

        Configuration config = new Configuration.Builder()
                .chunkSize(1024 * 1024)
                .putThreshold(4 * 1024 * 1024)
                .resumeUploadVersion(Configuration.RESUME_UPLOAD_VERSION_V1)
                .useConcurrentResumeUpload(false)
                .requestClient(new CurlClient())
                .zone(new FixedZone(new String[]{TestHost0, TestHost1}))
                .build();
        UploadManager manager = new UploadManager(config);
        manager.put(file, key, TestConfig.token_z0, new UpCompletionHandler() {
            @Override
            public void complete(String key, ResponseInfo responseInfo, JSONObject response) {
                info = new UploadCompleteInfo();
                info.key = key;
                info.responseInfo = responseInfo;
                info.response = response;
            }
        }, options);

        wait(new WaitConditional() {
            @Override
            public boolean shouldWait() {
                return info == null;
            }
        }, 10 * 60);

        assertTrue("=== responseInfo was null", info.responseInfo != null);
        assertTrue("=== responseInfo:" + info, info.responseInfo.isOK());
        assertTrue("=== upload key  was" + info.key, key.equals(info.key));
        assertTrue("=== response was null", info.response != null);
        String responseHash = info.response.getString("hash");
        String fileHash = Etag.file(file);
        assertTrue("=== file hash:" + responseHash, fileHash.equals(responseHash));
    }

    @Test
    public void resumeV2Upload() throws IOException  {
        String key = "resume_v2_5M.zip";
        File file = TempFile.createFile(1024 * 5, key);

        GlobalConfiguration.getInstance().dns = new Dns() {
            @Override
            public List<IDnsNetworkAddress> lookup(String hostname) throws UnknownHostException {
                if (hostname.equals(TestHost0) || hostname.equals(TestHost1)) {
                    List<IDnsNetworkAddress> list = new ArrayList<>();
                    list.add(new HostAddress(hostname, "111.1.36.180"));
                    list.add(new HostAddress(hostname, "218.98.28.87"));
                    list.add(new HostAddress(hostname, "112.13.172.42"));
                    return list;
                } else {
                    return null;
                }
            }
        };

        UploadOptions options = new UploadOptions(null, null, false, new UpProgressHandler() {
            @Override
            public void progress(String key, double percent) {
                System.out.println("=== curl progress key:" + key + " percent:" + percent);
            }
        }, null);

        Configuration config = new Configuration.Builder()
                .chunkSize(1024 * 1024)
                .putThreshold(4 * 1024 * 1024)
                .resumeUploadVersion(Configuration.RESUME_UPLOAD_VERSION_V2)
                .useConcurrentResumeUpload(false)
                .requestClient(new CurlClient())
                .zone(new FixedZone(new String[]{TestHost0, TestHost1}))
                .build();
        UploadManager manager = new UploadManager(config);
        manager.put(file, key, TestConfig.token_z0, new UpCompletionHandler() {
            @Override
            public void complete(String key, ResponseInfo responseInfo, JSONObject response) {
                info = new UploadCompleteInfo();
                info.key = key;
                info.responseInfo = responseInfo;
                info.response = response;
            }
        }, options);

        wait(new WaitConditional() {
            @Override
            public boolean shouldWait() {
                return info == null;
            }
        }, 10 * 60);

        assertTrue("=== responseInfo was null", info.responseInfo != null);
        assertTrue("=== responseInfo:" + info, info.responseInfo.isOK());
        assertTrue("=== upload key  was" + info.key, key.equals(info.key));
        assertTrue("=== response was null", info.response != null);
    }

    @Test
    public void cancelFormUploadTest() throws IOException {
        String key = "form_cancel_2M.zip";
        File file = TempFile.createFile(1024 * 2, key);

        GlobalConfiguration.getInstance().dns = new Dns() {
            @Override
            public List<IDnsNetworkAddress> lookup(String hostname) throws UnknownHostException {
                if (hostname.equals(TestHost0) || hostname.equals(TestHost1)) {
                    List<IDnsNetworkAddress> list = new ArrayList<>();
                    list.add(new HostAddress(hostname, "111.1.36.180"));
                    list.add(new HostAddress(hostname, "218.98.28.87"));
                    list.add(new HostAddress(hostname, "112.13.172.42"));
                    return list;
                } else {
                    return null;
                }
            }
        };

        final boolean[] shouldCancel = {false};
        UploadOptions options = new UploadOptions(null, null, false, new UpProgressHandler() {
            @Override
            public void progress(String key, double percent) {
                System.out.println("=== curl progress key:" + key + " percent:" + percent);
                if (percent > 0.2) {
                    shouldCancel[0] = true;
                }
            }
        }, new UpCancellationSignal() {
            @Override
            public boolean isCancelled() {
                return shouldCancel[0];
            }
        });

        Configuration config = new Configuration.Builder()
                .chunkSize(1024 * 1024)
                .putThreshold(4 * 1024 * 1024)
                .resumeUploadVersion(Configuration.RESUME_UPLOAD_VERSION_V1)
                .useConcurrentResumeUpload(false)
                .requestClient(new CurlClient())
                .zone(new FixedZone(new String[]{TestHost0, TestHost1}))
                .build();
        UploadManager manager = new UploadManager(config);
        manager.put(file, key, TestConfig.token_z0, new UpCompletionHandler() {
            @Override
            public void complete(String key, ResponseInfo responseInfo, JSONObject response) {
                info = new UploadCompleteInfo();
                info.key = key;
                info.responseInfo = responseInfo;
                info.response = response;
            }
        }, options);

        wait(new WaitConditional() {
            @Override
            public boolean shouldWait() {
                return info == null;
            }
        }, 10 * 60);

        assertTrue("=== responseInfo was null", info.responseInfo != null);
        assertTrue("=== responseInfo:" + info, info.responseInfo.isCancelled());
        assertTrue("=== upload key  was" + info.key, key.equals(info.key));
    }

    @Test
    public void cancelResumeUploadTest() throws IOException {
        String key = "resume_v1_cancel_5M.zip";
        File file = TempFile.createFile(1024 * 5, key);

        GlobalConfiguration.getInstance().dns = new Dns() {
            @Override
            public List<IDnsNetworkAddress> lookup(String hostname) throws UnknownHostException {
                if (hostname.equals(TestHost0) || hostname.equals(TestHost1)) {
                    List<IDnsNetworkAddress> list = new ArrayList<>();
                    list.add(new HostAddress(hostname, "111.1.36.180"));
                    list.add(new HostAddress(hostname, "218.98.28.87"));
                    list.add(new HostAddress(hostname, "112.13.172.42"));
                    return list;
                } else {
                    return null;
                }
            }
        };

        final boolean[] shouldCancel = {false};
        UploadOptions options = new UploadOptions(null, null, false, new UpProgressHandler() {
            @Override
            public void progress(String key, double percent) {
                System.out.println("=== curl progress key:" + key + " percent:" + percent);
                if (percent > 0.2) {
                    shouldCancel[0] = true;
                }
            }
        }, new UpCancellationSignal() {
            @Override
            public boolean isCancelled() {
                return shouldCancel[0];
            }
        });

        Configuration config = new Configuration.Builder()
                .chunkSize(1024 * 1024)
                .putThreshold(4 * 1024 * 1024)
                .resumeUploadVersion(Configuration.RESUME_UPLOAD_VERSION_V1)
                .useConcurrentResumeUpload(false)
                .requestClient(new CurlClient())
                .zone(new FixedZone(new String[]{TestHost0, TestHost1}))
                .build();
        UploadManager manager = new UploadManager(config);
        manager.put(file, key, TestConfig.token_z0, new UpCompletionHandler() {
            @Override
            public void complete(String key, ResponseInfo responseInfo, JSONObject response) {
                info = new UploadCompleteInfo();
                info.key = key;
                info.responseInfo = responseInfo;
                info.response = response;
            }
        }, options);

        wait(new WaitConditional() {
            @Override
            public boolean shouldWait() {
                return info == null;
            }
        }, 10 * 60);

        assertTrue("=== responseInfo was null", info.responseInfo != null);
        assertTrue("=== responseInfo:" + info, info.responseInfo.isCancelled());
        assertTrue("=== upload key  was" + info.key, key.equals(info.key));
    }

    @After
    public void completed() {
        System.out.println("=== test completed");
    }

    protected static class UploadCompleteInfo {
        String key;
        ResponseInfo responseInfo;
        JSONObject response;
    }

    private static class HostAddress implements IDnsNetworkAddress {
        private final String host;
        private final String ip;
        private final Long ttl = 120L;
        private final String sourceValue = "customized";
        private final Long timestamp = new Date().getTime() / 1000;

        HostAddress(String host, String ip) {
            this.host = host;
            this.ip = ip;
        }

        @Override
        public String getHostValue() {
            return host;
        }

        @Override
        public String getIpValue() {
            return ip;
        }

        @Override
        public Long getTtlValue() {
            return ttl;
        }

        @Override
        public String getSourceValue() {
            return sourceValue;
        }

        @Override
        public Long getTimestampValue() {
            return timestamp;
        }
    }
}
