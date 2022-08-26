package com.qiniu.client.curl;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import androidx.test.ext.junit.runners.AndroidJUnit4;

import com.qiniu.android.http.ResponseInfo;
import com.qiniu.android.http.metrics.UploadSingleRequestMetrics;
import com.qiniu.android.http.request.IRequestClient;
import com.qiniu.android.http.request.Request;

import org.json.JSONObject;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;

@RunWith(AndroidJUnit4.class)
public class CurlTest extends BaseTest {

    @Before
    public void setup() {
        System.out.println("\n=== curl test before");
    }

    @Test
    public void requestGet() {
        Request request = new Request("https://developer.qiniu.com/", Request.HttpMethodGet, null, null, 30);

        CurlClient client = new CurlClient();
        final boolean[] isCompleted = {false};
        final ResponseInfo[] res = {null};
        client.request(request, new IRequestClient.Options(null, true, null), null, new IRequestClient.CompleteHandler() {
            @Override
            public void complete(ResponseInfo responseInfo, UploadSingleRequestMetrics metrics, JSONObject response) {
                res[0] = responseInfo;
                isCompleted[0] = true;
            }
        });

        wait(new WaitConditional() {
            @Override
            public boolean shouldWait() {
                return !isCompleted[0];
            }
        }, 60);

        assertTrue("response was null" , res[0] != null);
        assertEquals("response:" + res[0], 200, res[0].statusCode);
    }

    @After
    public void completed() {
        System.out.println("=== curl test completed");
    }
}

