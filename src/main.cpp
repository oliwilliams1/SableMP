//#include <components/PlaylistView.h>
//int main()
//{
//	RegisterComponent<PlaylistView>("PlaylistView");
//
//	InitialisePrimaryWindow("SableMP", 1000, 700);
//
//	Panel("PlaylistView");
//
//	while (WaitEvents())
//		Render();
//
//	Shutdown();
//
//	return 0;
//}


#include <MediaLibrary/MediaLibrary.h>

int main()
{
	SableML::Library library;
	library.ScanDirectory("D:/Music/Library/TripleS/");

	return 0;
}