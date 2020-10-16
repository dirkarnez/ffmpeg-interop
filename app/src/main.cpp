#include <iostream>
#include <windows.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libswscale/swscale.h>
}


using namespace std;
typedef AVFormatContext* (__cdecl *__avformat_alloc_context)(void);
// typedef int (__cdecl *__avformat_open_input)(AVFormatContext **, const char *, AVInputFormat *, AVDictionary **);
// typedef int (__cdecl *__avformat_open_input)(AVFormatContext **, const char *, AVInputFormat *, AVDictionary **);


int main()
{
    //const char *ffp = "C:\\Users\\19081126D\\Downloads\\ffmpeg-4.3.1-2020-10-01-full_build-shared\\bin\\avformat-58.dll";
    const char *ffp = "C:\\Users\\19081126D\\Downloads\\ffmpeg-interop\\bin\\ffmpeg.dll";
    HINSTANCE hinstLib = LoadLibraryA(ffp);

    cout << hinstLib << endl;
    __avformat_alloc_context avformat_alloc_context = (__avformat_alloc_context)GetProcAddress(hinstLib, "avformat_alloc_context");

    //AVFormatContext* format_ctx = avformat_alloc_context();



//
//	avdevice_register_all();
//	avformat_network_init();

//    __avformat_open_input avformat_open_input = (__avformat_open_input)GetProcAddress(hinstLib, "avformat_open_input");
//
//    if (avformat_open_input(&m_pFormatContext, strFile.c_str(), iformat, &options) < 0)
//
//   cout  << avformat_open_input(NULL, NULL, NULL, NULL) << endl;


//    __avformat_open_input __avformat_open_input_proc  = (__avformat_open_input)GetProcAddress(hinstLib, "avformat_open_input");
//
//
//
//    cout  << __avformat_open_input_proc << endl;
//
//
//    ::AVFormatContext * ctx = NULL;
//    int err = __avformat_open_input_proc(&ctx, "g:\\Media\\The Sneezing Baby Panda.flv", NULL, NULL);

    system("pause");
    return 0;
}
