#include <SableUI/SableUI.h>

using namespace SableUI;
using namespace SableUI::Style;

struct Album
{
	SableString artist;
	SableString album;
	SableString year;
	Colour cover;

	std::vector<SableString> tracks;
};

class PlaylistView : public BaseComponent
{
public:
	PlaylistView()
	{
		albums =
		{
			{
				"yeule",
				"Ending",
				"2014",
				rgb(73, 91, 122),
				{
					"Ending",
					"Healing",
					"Impure",
					"Intermission",
					"Thoughts"
				}
			},

			{
				"KISS OF LIFE",
				"Midas Touch",
				"2024",
				rgb(205, 142, 64),
				{
					"Midas Touch",
					"Nothing"
				}
			},

			{
				"Laufey",
				"Everything I Know About Love",
				"2022",
				rgb(92, 36, 36),
				{
					"Fragile",
					"Beautiful Stranger",
					"Valentine",
					"Above the Chinese Restaurant",
					"Dear Soulmate",
					"What Love Will Do to You",
					"I’ve Never Been in Love Before",
					"Just Like Chet",
					"Everything I Know About Love",
					"Falling Behind",
					"Hi",
					"Dance With You Tonight",
					"Slow Down",
					"Lucky for Me",
					"Questions for the Universe"
				}
			},

			{
				"tripleS",
				U"EVOlution <⟡>",
				"2023",
				rgb(84, 104, 78),
				{
					U"⟡",
					"Invincible",
					"Rhodanthe",
					"Heavy Metal Wings",
					U"미열 37.5",
					"Moto Princess",
					"Oui",
					"Enhanced Flower"
				}
			},

			{
				"LOONA",
				"[12:00]",
				"2020",
				rgb(104, 32, 78),
				{
					"12:00",
					"Why Not?",
					U"목소리 (Voice)",
					U"기억해 (Fall Again)",
					"Universe",
					U"숨바꼭질 (Hide & Seek)",
					"OOPS!",
					U"Star (목소리 English ver.)"
				}
			}
		};
	}

	void Layout() override
	{
		auto& t = GetTheme();

		const int TRACK_NO_X = 18;
		const int DATE_X = 40;
		const int LENGTH_X = 50;

		// HEADER
		Div(w_fill, left_right)
		{
			RectElement(w(128 + 16));

			const int MARGIN_Y = 4;
			const int MARGIN_X = 2;

			RectElement(w(1), h_fill, bg(t.overlay0), mr(MARGIN_X));
			Text("#", w(TRACK_NO_X), justify_right, mx(MARGIN_X), my(MARGIN_Y));
			RectElement(w(1), h_fill, bg(t.overlay0), mx(MARGIN_X));

			Text("TITLE", w_fill, mx(MARGIN_X), my(MARGIN_Y));
			RectElement(w(1), h_fill, bg(t.overlay0), mx(MARGIN_X));

			Text("ARTIST", w_fill, mx(MARGIN_X), my(MARGIN_Y));
			RectElement(w(1), h_fill, bg(t.overlay0), mx(2));

			Text("ALBUM", w_fill, mx(MARGIN_X), my(MARGIN_Y));
			RectElement(w(1), h_fill, bg(t.overlay0), mx(MARGIN_X));

			Text("DATE", w(DATE_X), justify_right, mx(MARGIN_X), my(MARGIN_Y));
			RectElement(w(1), h_fill, bg(t.overlay0), mx(MARGIN_X));

			Text("LENGTH", w(LENGTH_X), justify_right, mx(MARGIN_X), my(MARGIN_Y));
			RectElement(w(1), h_fill, bg(t.overlay0), ml(MARGIN_X));

			RectElement(w(14), h_fill, bg(t.crust));
		}

		ScrollViewCtx(scrollCtx)
		{
			for (int i = 0; i < albums.size(); i++)
			{
				const Album& album = albums[i];

				// GROUP HEADER
				Div(w_fill, left_right, p(4))
				{
					Text(album.artist + " / " + album.year,
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
									w(TRACK_NO_X), justify_right, mx(4), textColour(rgba(255, 255, 255, 120)));

								Text(album.tracks[j],
									w_fill, mx(4));

								Text(album.artist,
									w_fill, mx(4), textColour(rgba(255, 255, 255, 170)));

								Text(album.album,
									w_fill, mx(4), textColour(rgba(255, 255, 255, 140)));

								Text(album.year,
									w(DATE_X), justify_right, mx(4), textColour(rgba(255, 255, 255, 120)));

								Text(SableString::Format("%d:%02d", minutes, seconds),
									w(LENGTH_X), justify_right, mx(4), textColour(rgba(255, 255, 255, 180)));
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