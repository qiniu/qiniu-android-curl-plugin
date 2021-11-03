package com.qiniu.client.curl;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import android.util.Log;

import com.qiniu.android.http.ResponseInfo;
import com.qiniu.android.http.metrics.UploadSingleRequestMetrics;
import com.qiniu.android.http.request.IRequestClient;
import com.qiniu.android.http.request.Request;

import org.json.JSONObject;
import org.junit.Test;

public class CurlTest extends BaseTest {

    @Test
    public void requestGet() {
        Request request = new Request("https://developer.qiniu.com/", Request.HttpMethodGet, null, null, 30);

        CurlClient client = new CurlClient();
        final boolean[] isCompleted = {false};
        final ResponseInfo[] res = {null};
        client.request(request, null, true, null, null, new IRequestClient.RequestClientCompleteHandler() {
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
}

