#include <SableUI/SableUI.h>
#include <MediaLibrary/MediaLibrary.h>
#include <algorithm>

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
		const std::vector<SableML::Track>* tracks = SableML::Library::GetInstance().GetTracks();

		struct AlbumKey
		{
			std::string album;
			std::string albumArtist;

			bool operator==(const AlbumKey& other) const
			{
				return album == other.album &&
					albumArtist == other.albumArtist;
			}
		};

		struct AlbumKeyHasher
		{
			size_t operator()(const AlbumKey& k) const
			{
				return std::hash<std::string>()(k.album) ^
					(std::hash<std::string>()(k.albumArtist) << 1);
			}
		};

		struct AlbumGroup
		{
			const SableML::Track* representative = nullptr;
			std::vector<const SableML::Track*> tracks;
		};

		std::unordered_map<AlbumKey, AlbumGroup, AlbumKeyHasher> grouped;

		for (const SableML::Track& track : *tracks)
		{
			const auto& md = track.metadata;

			std::string album = md.album.empty() ? "Unknown Album" : md.album;

			std::string albumArtist =
				!md.albumArtist.empty() ? md.albumArtist :
				!md.artist.empty() ? md.artist :
				"Unknown Artist";

			AlbumKey key
			{
				album,
				albumArtist
			};

			auto& group = grouped[key];

			if (!group.representative)
				group.representative = &track;

			group.tracks.push_back(&track);
		}

		std::vector<std::pair<AlbumKey, AlbumGroup*>> sortedAlbums;

		for (auto& [key, group] : grouped)
			sortedAlbums.push_back({ key, &group });

		std::sort(sortedAlbums.begin(), sortedAlbums.end(),
			[](const auto& a, const auto& b)
			{
				return a.first.album < b.first.album;
			});

		for (auto& [key, group] : sortedAlbums)
		{
			std::sort(group->tracks.begin(), group->tracks.end(),
				[](const SableML::Track* a, const SableML::Track* b)
				{
					const auto& ma = a->metadata;
					const auto& mb = b->metadata;

					if (ma.discNumber != mb.discNumber)
						return ma.discNumber < mb.discNumber;

					if (ma.trackNumber != mb.trackNumber)
						return ma.trackNumber < mb.trackNumber;

					return ma.title < mb.title;
				});

			const auto& rep = group->representative->metadata;

			Album album;

			album.artist =
				!rep.albumArtist.empty() ? rep.albumArtist :
				!rep.artist.empty() ? rep.artist :
				"Unknown Artist";

			album.album =
				!rep.album.empty() ? rep.album :
				"Unknown Album";

			album.year =
				rep.year != 0 ?
				SableString::Format("%u", rep.year) :
				"----";

			album.cover = rgb(
				60 + (albums.size() * 23) % 120,
				60 + (albums.size() * 47) % 120,
				60 + (albums.size() * 71) % 120);

			for (const SableML::Track* track : group->tracks)
			{
				const auto& md = track->metadata;

				album.tracks.push_back(
					!md.title.empty() ?
					md.title :
					"Unknown Track");
			}

			albums.push_back(std::move(album));
		}
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