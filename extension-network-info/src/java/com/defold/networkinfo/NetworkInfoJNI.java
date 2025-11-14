package com.defold.networkinfo;

import android.util.Log;

import java.net.ProxySelector;
import java.net.URI;
import java.net.Proxy;
import java.net.Proxy.Type;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.net.URISyntaxException;

import java.util.List;

import org.json.JSONArray;
import org.json.JSONObject;
import org.json.JSONException;


public class NetworkInfoJNI {

    private static final String TAG = "NetworkInfoJNI";

    public NetworkInfoJNI() {
    }

    public String getProxy(String url) {

        JSONArray jsonproxies = new JSONArray();
        try {
            ProxySelector proxySelector = ProxySelector.getDefault();
            List<Proxy> proxies = proxySelector.select(new URI(url));
            for (Proxy proxy : proxies) {
                JSONObject jsonproxy = new JSONObject();

                switch (proxy.type()) {
                case DIRECT:
                    jsonproxy.put("type", "direct");
                    break;
                case HTTP:
                    jsonproxy.put("type", "http");
                    break;
                case SOCKS:
                    jsonproxy.put("type", "socks");
                    break;
                default:
                    jsonproxy.put("type", "unknown");
                    break;
                }

                SocketAddress addr = proxy.address();
                if (addr instanceof InetSocketAddress) {
                    jsonproxy.put("port", ((InetSocketAddress)addr).getPort());
                    jsonproxy.put("host", ((InetSocketAddress)addr).getHostString());
                }
                jsonproxies.put(jsonproxy);
            }
        }
        catch (JSONException|URISyntaxException e) {
            Log.e(TAG, e.getMessage());
        }
        return jsonproxies.toString();
    }
}
