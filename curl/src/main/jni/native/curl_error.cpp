//
// Created by yangsen on 2020/11/16.
//

#include "curl_error.h"
#include "curl/curl.h"

int transformCurlStatusCode(int statusCode){

    int statusCodeRet = kNetworkError;
    switch (statusCode){
        case CURLE_OK: // 0
            // All fine. Proceed as usual.
            statusCodeRet = 0;
            break;
        case CURLE_UNSUPPORTED_PROTOCOL: // 1
            // The URL you passed to libcurl used a protocol that this libcurl does not support.
            // The support might be a compile-time option that you didn't use, it can be a
            // misspelled protocol string or just a protocol libcurl has no code for.
            statusCodeRet = kNetworkProtocolError;
            break;
        case CURLE_FAILED_INIT: // 2
            // Very early initialization code failed. This is likely to be an internal error or
            // problem, or a resource problem where something fundamental couldn't get done at init
            // time.
            break;
        case CURLE_URL_MALFORMAT: // 3
            // The URL was not properly formatted.
            break;
        case CURLE_NOT_BUILT_IN: // 4
            // A requested feature, protocol or option was not found built-in in this libcurl due
            // to a build-time decision. This means that a feature or option was not enabled or
            // explicitly disabled when libcurl was built and in order to get it to function you
            // have to get a rebuilt libcurl.
            statusCodeRet = kLocalIOError;
            break;
        case CURLE_COULDNT_RESOLVE_PROXY: // 5
            // Couldn't resolve proxy. The given proxy host could not be resolved.
            break;
        case CURLE_COULDNT_RESOLVE_HOST: // 6
            // Couldn't resolve host. The given remote host was not resolved.
            statusCodeRet = kUnknownHost;
            break;
        case CURLE_COULDNT_CONNECT: // 7
            // Failed to connect() to host or proxy.
            statusCodeRet = kCannotConnectToHost;
            break;
        case CURLE_WEIRD_SERVER_REPLY: // 8
            // The server sent data libcurl couldn't parse. This error code was known as as
            // CURLE_FTP_WEIRD_SERVER_REPLY before 7.51.0.
            statusCodeRet = kParseError;
            break;
        case CURLE_REMOTE_ACCESS_DENIED: // 9
            // We were denied access to the resource given in the URL. For FTP, this occurs while
            // trying to change to the remote directory.
            break;
        case CURLE_FTP_ACCEPT_FAILED: // 10
            // While waiting for the server to connect back when an active FTP session is used, an
            // error code was sent over the control connection or similar.
            break;
        case CURLE_FTP_WEIRD_PASS_REPLY: // 11
            // After having sent the FTP password to the server, libcurl expects a proper reply.
            // This error code indicates that an unexpected code was returned.
            break;
        case CURLE_FTP_ACCEPT_TIMEOUT: // 12
            // During an active FTP session while waiting for the server to connect, the
            // CURLOPT_ACCEPTTIMEOUT_MS (or the internal default) timeout expired.
            break;
        case CURLE_FTP_WEIRD_PASV_REPLY: // 13
            // libcurl failed to get a sensible result back from the server as a response to
            // either a PASV or a EPSV command. The server is flawed.
            break;
        case CURLE_FTP_WEIRD_227_FORMAT: // 14
            // FTP servers return a 227-line as a response to a PASV command. If libcurl fails to
            // parse that line, this return code is passed back.
            break;
        case CURLE_FTP_CANT_GET_HOST: // 15
            // An internal failure to lookup the host used for the new connection.
            break;
        case CURLE_HTTP2: // 16
            // A problem was detected in the HTTP2 framing layer. This is somewhat generic and can
            // be one out of several problems, see the error buffer for details.
            statusCodeRet = kLocalIOError;
            break;
        case CURLE_FTP_COULDNT_SET_TYPE: // 17
            // Received an error when trying to set the transfer mode to binary or ASCII.
            break;
        case CURLE_PARTIAL_FILE: // 18
            // A file transfer was shorter or larger than expected. This happens when the server
            // first reports an expected transfer size, and then delivers data that doesn't match
            // the previously given size.
            statusCodeRet = kLocalIOError;
            break;
        case CURLE_FTP_COULDNT_RETR_FILE: // 19
            // This was either a weird reply to a 'RETR' command or a zero byte transfer complete.
            break;
        case CURLE_QUOTE_ERROR: // 21
            // When sending custom "QUOTE" commands to the remote server, one of the commands
            // returned an error code that was 400 or higher (for FTP) or otherwise indicated
            // unsuccessful completion of the command.
            break;
        case CURLE_HTTP_RETURNED_ERROR: // 22
            // This is returned if CURLOPT_FAILONERROR is set TRUE and the HTTP server returns an
            // error code that is >= 400.
            break;
        case CURLE_WRITE_ERROR: // 23
            // An error occurred when writing received data to a local file, or an error was
            // returned to libcurl from a write callback.
            statusCodeRet = kLocalIOError;
            break;
        case CURLE_UPLOAD_FAILED: // 25
            // Failed starting the upload. For FTP, the server typically denied the STOR command.
            // The error buffer usually contains the server's explanation for this.
            break;
        case CURLE_READ_ERROR: // 26
            // There was a problem reading a local file or an error returned by the read callback.
            statusCodeRet = kLocalIOError;
            break;
        case CURLE_OUT_OF_MEMORY: // 27
            // A memory allocation request failed. This is serious badness and things are severely
            // screwed up if this ever occurs.
            statusCodeRet = kLocalIOError;
            break;
        case CURLE_OPERATION_TIMEDOUT: // 28
            // Operation timeout. The specified time-out period was reached according to the
            // conditions.
            statusCodeRet = kTimedOut;
            break;
        case CURLE_FTP_PORT_FAILED: // 30
            // The FTP PORT command returned error. This mostly happens when you haven't specified
            // a good enough address for libcurl to use. See CURLOPT_FTPPORT.
            break;
        case CURLE_FTP_COULDNT_USE_REST: // 31
            // The FTP REST command returned error. This should never happen if the server is sane.
            break;
        case CURLE_RANGE_ERROR: // 33
            // The server does not support or accept range requests.
            statusCodeRet = kLocalIOError;
            break;
        case CURLE_HTTP_POST_ERROR: // 34
            // This is an odd error that mainly occurs due to internal confusion.
            break;
        case CURLE_SSL_CONNECT_ERROR: // 35
            // A problem occurred somewhere in the SSL/TLS handshake. You really want the error
            // buffer and read the message there as it pinpoints the problem slightly more. Could
            // be certificates (file formats, paths, permissions), passwords, and others.
            statusCodeRet = kNetworkSSLError;
            break;
        case CURLE_BAD_DOWNLOAD_RESUME: // 36
            // The download could not be resumed because the specified offset was out of the file boundary.
            break;
        case CURLE_FILE_COULDNT_READ_FILE: // 37
            // A file given with FILE:// couldn't be opened. Most likely because the file path
            // doesn't identify an existing file. Did you check file permissions?
            statusCodeRet = kLocalIOError;
            break;
        case CURLE_LDAP_CANNOT_BIND: // 38
            // LDAP cannot bind. LDAP bind operation failed.
            break;
        case CURLE_LDAP_SEARCH_FAILED: // 39
            // LDAP search failed.
            break;
        case CURLE_FUNCTION_NOT_FOUND: // 41
            // Function not found. A required zlib function was not found.
            break;
        case CURLE_ABORTED_BY_CALLBACK: // 42
            // Aborted by callback. A callback returned "abort" to libcurl.
            break;
        case CURLE_BAD_FUNCTION_ARGUMENT: // 43
            // A function was called with a bad parameter.
            break;
        case CURLE_INTERFACE_FAILED: // 45
            // Interface error. A specified outgoing interface could not be used. Set which
            // interface to use for outgoing connections' source IP address with CURLOPT_INTERFACE.
            break;
        case CURLE_TOO_MANY_REDIRECTS: // 47
            // Too many redirects. When following redirects, libcurl hit the maximum amount.
            // Set your limit with CURLOPT_MAXREDIRS.
            break;
        case CURLE_UNKNOWN_OPTION: // 48
            // An option passed to libcurl is not recognized/known. Refer to the appropriate
            // documentation. This is most likely a problem in the program that uses libcurl.
            // The error buffer might contain more specific information about which exact option
            // it concerns.
            break;
        case CURLE_TELNET_OPTION_SYNTAX: // 49
            // A telnet option string was Illegally formatted.
            break;
        case CURLE_GOT_NOTHING: // 52
            // Nothing was returned from the server, and under the circumstances, getting nothing
            // is considered an error.
            break;
        case CURLE_SSL_ENGINE_NOTFOUND: // 53
            // The specified crypto engine wasn't found.
            statusCodeRet = kNetworkSSLError;
            break;
        case CURLE_SSL_ENGINE_SETFAILED: // 54
            // Failed setting the selected SSL crypto engine as default!
            statusCodeRet = kNetworkSSLError;
            break;
        case CURLE_SEND_ERROR: // 55
            // Failed sending network data.
            statusCodeRet = kNetworkConnectionLost;
            break;
        case CURLE_RECV_ERROR: // 56
            // Failure with receiving network data.
            statusCodeRet = kNetworkConnectionLost;
            break;
        case CURLE_SSL_CERTPROBLEM: // 58
            // problem with the local client certificate.
            statusCodeRet = kNetworkSSLError;
            break;
        case CURLE_SSL_CIPHER: // 59
            // Couldn't use specified cipher.
            statusCodeRet = kNetworkSSLError;
            break;
        case CURLE_PEER_FAILED_VERIFICATION: // 60
            // The remote server's SSL certificate or SSH md5 fingerprint was deemed not OK. This
            // error code has been unified with CURLE_SSL_CACERT since 7.62.0. Its previous value
            // was 51.
            statusCodeRet = kNetworkSSLError;
            break;
        case CURLE_BAD_CONTENT_ENCODING: // 61
            // Unrecognized transfer encoding.
            break;
        case CURLE_LDAP_INVALID_URL: // 62
            // Invalid LDAP URL.
            break;
        case CURLE_FILESIZE_EXCEEDED: // 63
            // Maximum file size exceeded.
            statusCodeRet = kLocalIOError;
            break;
        case CURLE_USE_SSL_FAILED: // 64
            // Requested FTP SSL level failed.
            statusCodeRet = kNetworkSSLError;
            break;
        case CURLE_SEND_FAIL_REWIND: // 65
            // When doing a send operation curl had to rewind the data to retransmit, but the
            // rewinding operation failed.
            break;
        case CURLE_SSL_ENGINE_INITFAILED: // 66
            // Initiating the SSL Engine failed.
            statusCodeRet = kNetworkSSLError;
            break;
        case CURLE_LOGIN_DENIED: // 67
            // The remote server denied curl to login (Added in 7.13.1)
            break;
        case CURLE_TFTP_NOTFOUND: // 68
            // File not found on TFTP server.
            break;
        case CURLE_TFTP_PERM: // 69
            // Permission problem on TFTP server.
            break;
        case CURLE_REMOTE_DISK_FULL: // 70
            // Out of disk space on the server.
            break;
        case CURLE_TFTP_ILLEGAL: // 71
            // Illegal TFTP operation.
            break;
        case CURLE_TFTP_UNKNOWNID: // 72
            // Unknown TFTP transfer ID.
            break;
        case CURLE_REMOTE_FILE_EXISTS: // 73
            // File already exists and will not be overwritten.
            break;
        case CURLE_TFTP_NOSUCHUSER: // 74
            // This error should never be returned by a properly functioning TFTP server.
            break;
        case CURLE_CONV_FAILED: // 75
            // Character conversion failed.
            break;
        case CURLE_CONV_REQD: // 76
            // Caller must register conversion callbacks.
            break;
        case CURLE_SSL_CACERT_BADFILE: // 77
            // Problem with reading the SSL CA cert (path? access rights?)
            statusCodeRet = kNetworkSSLError;
            break;
        case CURLE_REMOTE_FILE_NOT_FOUND: // 78
            // The resource referenced in the URL does not exist.
            break;
        case CURLE_SSH: // 79
            // An unspecified error occurred during the SSH session.
            break;
        case CURLE_SSL_SHUTDOWN_FAILED: // 80
            // Failed to shut down the SSL connection.
            break;
        case CURLE_AGAIN: // 81
            // Socket is not ready for send/recv wait till it's ready and try again. This return
            // code is only returned from curl_easy_recv and curl_easy_send (Added in 7.18.2)
            break;
        case CURLE_SSL_CRL_BADFILE: // 82
            // Failed to load CRL file (Added in 7.19.0)
            statusCodeRet = kNetworkSSLError;
            break;
        case CURLE_SSL_ISSUER_ERROR: // 83
            // Issuer check failed (Added in 7.19.0)
            statusCodeRet = kNetworkSSLError;
            break;
        case CURLE_FTP_PRET_FAILED: // 84
            // The FTP server does not understand the PRET command at all or does not support the
            // given argument. Be careful when using CURLOPT_CUSTOMREQUEST, a custom LIST command
            // will be sent with PRET CMD before PASV as well. (Added in 7.20.0)
            break;
        case CURLE_RTSP_CSEQ_ERROR: // 85
            // Mismatch of RTSP CSeq numbers.
            break;
        case CURLE_RTSP_SESSION_ERROR: // 86
            // Mismatch of RTSP Session Identifiers.
            break;
        case CURLE_FTP_BAD_FILE_LIST: // 87
            // Unable to parse FTP file list (during FTP wildcard downloading).
            break;
        case CURLE_CHUNK_FAILED: // 88
            // Chunk callback reported error.
            break;
        case CURLE_NO_CONNECTION_AVAILABLE: // 89
            // (For internal use only, will never be returned by libcurl) No connection available,
            // the session will be queued. (added in 7.30.0)
            break;
        case CURLE_SSL_PINNEDPUBKEYNOTMATCH: // 90
            // Failed to match the pinned key specified with CURLOPT_PINNEDPUBLICKEY.
            statusCodeRet = kNetworkSSLError;
            break;
        case CURLE_SSL_INVALIDCERTSTATUS: // 91
            // Status returned failure when asked with CURLOPT_SSL_VERIFYSTATUS.
            statusCodeRet = kNetworkSSLError;
            break;
        case CURLE_HTTP2_STREAM: // 92
            // Stream error in the HTTP/2 framing layer.
            break;
        case CURLE_RECURSIVE_API_CALL: // 93
            // An API function was called from inside a callback.
            break;
        case CURLE_AUTH_ERROR: // 94
            // An authentication function returned an error.
            break;
        case CURLE_HTTP3: // 95
            // A problem was detected in the HTTP/3 layer. This is somewhat generic and can be one
            // out of several problems, see the error buffer for details.
            break;
//        case CURLE_QUIC_CONNECT_ERROR: // 96
            // QUIC connection error. This error may be caused by an SSL library error. QUIC is the
            // protocol used for HTTP/3 transfers.
//            break;
        default:
            break;
    }
    return statusCodeRet;
}