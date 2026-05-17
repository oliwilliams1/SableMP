#include <SableUI/SableUI.h>

using namespace SableUI;
using namespace SableUI::Style;

struct Album
{
	const char* artist;
	const char* album;
	const char* year;
	Colour cover;

	std::vector<const char*> tracks;
};

class PlaylistView : public BaseComponent
{
public:
	PlaylistView()
	{
		albums =
		{
			{
				"Radiohead",
				"OK Computer",
				"1997",
				rgb(73, 91, 122),
				{
					"Airbag",
					"Paranoid Android",
					"Subterranean Homesick Alien",
					"Exit Music (For A Film)",
					"Let Down",
					"No Surprises"
				}
			},

			{
				"Daft Punk",
				"Discovery",
				"2001",
				rgb(205, 142, 64),
				{
					"One More Time",
					"Aerodynamic",
					"Digital Love",
					"Harder Better Faster Stronger",
					"Crescendolls",
					"Face To Face"
				}
			},

			{
				"Massive Attack",
				"Mezzanine",
				"1998",
				rgb(92, 36, 36),
				{
					"Angel",
					"Risingson",
					"Teardrop",
					"Inertia Creeps",
					"Exchange",
					"Group Four"
				}
			},

			{
				"Boards of Canada",
				"Music Has The Right To Children",
				"1998",
				rgb(84, 104, 78),
				{
					"Roygbiv",
					"Aquarius",
					"Olson",
					"Pete Standing Alone",
					"Telephasic Workshop",
					"Happy Cycling"
				}
			}
		};
	}

	void Layout() override
	{
		// HEADER
		Div(w_fill, left_right, p(6), bg(rgb(22, 22, 22)))
		{
			RectElement(w(128 + 16));

			Text("#", w(24), justify_right, mx(4));
			Text("TITLE", w_fill, mx(4));
			Text("ARTIST", w_fill, mx(4));
			Text("ALBUM", w_fill, mx(4));
			Text("DATE", w(50), justify_right, mx(4));
			Text("LENGTH", w(60), justify_right, mx(4), mr(20));
		}

		ScrollViewCtx(scrollCtx)
		{
			for (int i = 0; i < albums.size(); i++)
			{
				const Album& album = albums[i];

				// GROUP HEADER
				Div(w_fill, left_right, p(4))
				{
					Text(SableString::Format("%s / %s", album.artist, album.year),
						textWrap(false), w_fit, textColour(rgba(255, 255, 255, 180)));

					RectElement(mx(6), bg(rgb(43, 43, 43)), h(1), w_fill, centerY);
				}

				Div(w_fill, left_right)
				{
					// ALBUM ART + INFO
					Div(up_down, w_fit, h_fit, mx(8))
					{
						RectElement(w(128), h(128), bg(album.cover), br(4));

						Text(album.album,
							w(128), textWrap(false), textColour(rgba(255, 255, 255, 220)), mt(4));

						Text(SableString::Format("%d tracks", (int)album.tracks.size()),
							w(128), fontSize(11), textColour(rgba(255, 255, 255, 120)));

						Div(py(4));
					}

					// TRACK LIST
					Div(w_fill, up_down)
					{
						for (int j = 0; j < album.tracks.size(); j++)
						{
							int minutes = 2 + ((i + j) % 5);
							int seconds = (17 * (i + j + 3)) % 60;

							Div(w_fill, left_right, py(2), bg(j % 2 == 0 ? rgba(255, 255, 255, 4) : rgba(0, 0, 0, 0)))
							{
								Text(SableString::Format("%d", j + 1),
									w(24), justify_right, mx(4), textColour(rgba(255, 255, 255, 120)));

								Text(album.tracks[j],
									w_fill, mx(4));

								Text(album.artist,
									w_fill, mx(4), textColour(rgba(255, 255, 255, 170)));

								Text(album.album,
									w_fill, mx(4), textColour(rgba(255, 255, 255, 140)));

								Text(album.year,
									w(50), justify_right, mx(4), textColour(rgba(255, 255, 255, 120)));

								Text(SableString::Format("%d:%02d", minutes, seconds),
									w(60), justify_right, mx(4), mr(20), textColour(rgba(255, 255, 255, 180)));
							}
						}
					}
				}
			}
		}
	}

	void OnUpdate(const UIUpdateContext& ctx) override
	{
		ScrollUpdateHandler(scrollCtx, ctx);
	}

	void OnUpdatePostLayout(const UIUpdateContext& ctx) override
	{
		ScrollUpdatePostLayoutHandler(scrollCtx);
	}

private:
	std::vector<Album> albums;
	ScrollContext scrollCtx;
};

int main()
{
	RegisterComponent<PlaylistView>("PlaylistView");

	InitialisePrimaryWindow("SableMP", 1000, 700);

	Panel("PlaylistView");

	while (WaitEvents())
		Render();

	Shutdown();

	return 0;
}