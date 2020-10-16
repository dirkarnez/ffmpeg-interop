//#include <iostream>
//#include <windows.h>
//
//extern "C" {
//#include <libavcodec/avcodec.h>
//#include <libavformat/avformat.h>
//#include <libavformat/avio.h>
//#include <libswscale/swscale.h>
//}
//
//
//using namespace std;
//typedef AVFormatContext* (__cdecl *__avformat_alloc_context)(void);
//// typedef int (__cdecl *__avformat_open_input)(AVFormatContext **, const char *, AVInputFormat *, AVDictionary **);
//// typedef int (__cdecl *__avformat_open_input)(AVFormatContext **, const char *, AVInputFormat *, AVDictionary **);
//
//
//int main()
//{
//    //const char *ffp = "C:\\Users\\19081126D\\Downloads\\ffmpeg-4.3.1-2020-10-01-full_build-shared\\bin\\avformat-58.dll";
//    const char *ffp = "C:\\Users\\19081126D\\Downloads\\ffmpeg-interop\\bin\\ffmpeg.dll";
//    HINSTANCE hinstLib = LoadLibraryA(ffp);
//
//    cout << hinstLib << endl;
//    __avformat_alloc_context avformat_alloc_context = (__avformat_alloc_context)GetProcAddress(hinstLib, "avformat_alloc_context");
//
//    //AVFormatContext* format_ctx = avformat_alloc_context();
//
//
//
////
////	avdevice_register_all();
////	avformat_network_init();
//
////    __avformat_open_input avformat_open_input = (__avformat_open_input)GetProcAddress(hinstLib, "avformat_open_input");
////
////    if (avformat_open_input(&m_pFormatContext, strFile.c_str(), iformat, &options) < 0)
////
////   cout  << avformat_open_input(NULL, NULL, NULL, NULL) << endl;
//
//
////    __avformat_open_input __avformat_open_input_proc  = (__avformat_open_input)GetProcAddress(hinstLib, "avformat_open_input");
////
////
////
////    cout  << __avformat_open_input_proc << endl;
////
////
////    ::AVFormatContext * ctx = NULL;
////    int err = __avformat_open_input_proc(&ctx, "g:\\Media\\The Sneezing Baby Panda.flv", NULL, NULL);
//
//    system("pause");
//    return 0;
//}


#include <iostream>

using namespace std;

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"
}

int main()
{
    const char* url = "https://nowcom-vodstream-hk.viu.com/getnowmediahls/Pt8O2wtbV5MVtw0a_UzNwQ/1602875834/NNEW00303517/sd/index.m3u8";
    cout << "Hello FFmpeg!" << endl;

    unsigned int version = avcodec_version();
    cout << "FFmpeg version is:" << version << endl;

    // Open the initial context variables that are needed
    AVFormatContext* format_ctx = avformat_alloc_context();

    avformat_network_init();

    if (avformat_open_input(&format_ctx, url,
        NULL, NULL) != 0) {
        return EXIT_FAILURE;
    }


    AVOutputFormat* outfmt = NULL;
    outfmt = av_guess_format(NULL, "111.mp4", NULL);

    format_ctx->oformat = outfmt;

    cout << "oformat:" << format_ctx->oformat->extensions << endl;


    AVStream* m_in_vid_strm = NULL;
    AVStream* m_in_aud_strm = NULL;

    unsigned int m_in_vid_strm_idx = -1;
    unsigned int m_in_aud_strm_idx = -1;

    for (unsigned int i = 0; i < format_ctx->nb_streams; i++)
    {
        if (format_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            cout << "Found Video Stream " << i << endl;

            m_in_vid_strm = format_ctx->streams[i];
            m_in_vid_strm_idx = i;
        }

        if (format_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            cout << "Found Audio Stream " << i << endl;
            m_in_aud_strm = format_ctx->streams[i];
            m_in_aud_strm_idx = i;
        }
    }

    AVCodec *out_vid_codec = NULL; 
    AVCodec *out_aud_codec = NULL;

    AVStream* m_out_vid_strm = NULL;
    AVStream* m_out_aud_strm = NULL;

    if (outfmt->video_codec != AV_CODEC_ID_NONE && m_in_vid_strm != NULL)
    {
        out_vid_codec = avcodec_find_encoder(outfmt->video_codec);
        if (NULL == out_vid_codec)
        {
            cout << "Could Not Find Vid Encoder" << endl;
        }
        else
        {
            cout << "Found Out Vid Encoder " << endl;
                m_out_vid_strm = avformat_new_stream(format_ctx, out_vid_codec);
            if (NULL == m_out_vid_strm)
            {
                cout << "Failed to Allocate Output Vid Strm " << endl;
            }
            else
            {
                cout << "Allocated Video Stream " << endl;
                    if (avcodec_copy_context(m_out_vid_strm->codec,
                        format_ctx->streams[m_in_vid_strm_idx]->codec) != 0)
                    {

                        cout << "Failed to Copy Context " << endl;
                    }
                    else
                    {
                        m_out_vid_strm->sample_aspect_ratio.den =
                            m_out_vid_strm->codec->sample_aspect_ratio.den;

                        m_out_vid_strm->sample_aspect_ratio.num =
                            m_in_vid_strm->codec->sample_aspect_ratio.num;
                        cout << "Copied Context " << endl;
                         m_out_vid_strm->codec->codec_id = m_in_vid_strm->codec->codec_id;
                        m_out_vid_strm->codec->time_base.num = 1;
                        m_out_vid_strm->codec->time_base.den = m_fps * (m_in_vid_strm->codec->ticks_per_frame);
                        m_out_vid_strm->time_base.num = 1;
                        m_out_vid_strm->time_base.den = 1000;
                        m_out_vid_strm->r_frame_rate.num = m_fps;
                        m_out_vid_strm->r_frame_rate.den = 1;
                        m_out_vid_strm->avg_frame_rate.den = 1;
                        m_out_vid_strm->avg_frame_rate.num = m_fps;
                        m_out_vid_strm->duration = (m_out_end_time - m_out_start_time) * 1000;
                    }
            }
        }
    }

    if (outfmt->audio_codec != AV_CODEC_ID_NONE && m_in_aud_strm != NULL)
    {
        out_aud_codec = avcodec_find_encoder(outfmt->audio_codec);
        if (NULL == out_aud_codec)
        {
            PRINT_MSG("Could Not Find Out Aud Encoder ")
                ret = -1;
            return ret;
        }
        else
        {
            PRINT_MSG("Found Out Aud Encoder ")
                m_out_aud_strm = avformat_new_stream(m_outformat, out_aud_codec);
            if (NULL == m_out_aud_strm)
            {
                PRINT_MSG("Failed to Allocate Out Vid Strm ")
                    ret = -1;
                return ret;
            }
            else
            {
                if (avcodec_copy_context(m_out_aud_strm->codec,
                    m_informat->streams[m_in_aud_strm_idx]->codec) != 0)
                {
                    PRINT_MSG("Failed to Copy Context ")
                        ret = -1;
                    return ret;
                }
                else
                {
                    PRINT_MSG("Copied Context ")
                        m_out_aud_strm->codec->codec_id = m_in_aud_strm->codec->codec_id;
                    m_out_aud_strm->codec->codec_tag = 0;
                    m_out_aud_strm->pts = m_in_aud_strm->pts;
                    m_out_aud_strm->duration = m_in_aud_strm->duration;
                    m_out_aud_strm->time_base.num = m_in_aud_strm->time_base.num;
                    m_out_aud_strm->time_base.den = m_in_aud_strm->time_base.den;

                }
            }
        }
    }

    system("pause");

    return 0;
}