#include "CPlayer.h"

const DWORD MAX_PLAYLIST_SEGMENTS = 128;

// For this example, there is a hard-coded limit to the number of playlist 
// segments, to avoid using a dynamically sized list.

class CPlaylist : public CPlayer
{
private:
    IMFSequencerSource* m_pSequencerSource;
    IMFPresentationClock* m_pPresentationClock;

    MFTIME      m_PresentationTimeOffset;
    DWORD       m_ActiveSegment;
    LONGLONG    m_hnsSegmentDuration;

    // Table of segment IDs and topology IDs.
    struct
    {
        MFSequencerElementId    SegmentID;
        TOPOID                    TopoID;
        IMFMediaSource*         pMediaSource;
    } m_segments[MAX_PLAYLIST_SEGMENTS];

    DWORD m_count;

public:

    MFSequencerElementId LastSegment() const
    {
        return m_segments[m_count - 1].SegmentID;
    }

    HRESULT LookupTopoID(TOPOID id, DWORD* pIndex);
    HRESULT OnSessionEvent(IMFMediaEvent* pEvent, MediaEventType meType);
    HRESULT OnTopologyStatus(IMFMediaEvent* pEvent);
    HRESULT OnNewPresentation(IMFMediaEvent* pEvent);
    HRESULT OnPresentationEnded(IMFMediaEvent* pEvent);
    HRESULT AddSegment(PCWSTR pszURL);
    HRESULT ExtendLast();
    HRESULT QueueNextSegment(IMFPresentationDescriptor* pPD);

    CPlaylist(HWND hVideo, HWND hEvent);
    ~CPlaylist();

    HRESULT Initialize2();

public:

    static HRESULT CreateInstance(HWND hVideo,                  // Video window.
        HWND hEvent,                  // Window to receive notifications.
        CPlaylist** ppPlayer);

    HRESULT AddToPlaylist(PCWSTR pszURL);
    HRESULT DeleteSegment(DWORD index);
    HRESULT SkipTo(DWORD index);

    DWORD   NumSegments() { return m_count; }
    DWORD   ActiveSegment() const { return m_ActiveSegment; }

    HRESULT GetPlaybackTime(MFTIME* phnsPresentation, MFTIME* phnsSegment);

    LONGLONG SegmentDuration() { return m_hnsSegmentDuration; }
};