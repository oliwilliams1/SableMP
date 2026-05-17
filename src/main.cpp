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
#include <components/PlaylistView.h>

int main()
{
	SableML::Library::Initialise();
	SableML::Library& library = SableML::Library::GetInstance();

	library.ScanDirectory("C:/Users/Oli/Music/Library/");

	SableUI::RegisterComponent<PlaylistView>("PlaylistView");

	SableUI::InitialisePrimaryWindow("SableMP", 1000, 700);
	
	Panel("PlaylistView");
	
	while (SableUI::WaitEvents())
		SableUI::Render();
	
	SableUI::Shutdown();


	SableML::Library::Shutdown();

	return 0;
}