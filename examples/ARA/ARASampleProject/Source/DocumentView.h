#pragma once

#include "JuceHeader.h"

class RulersView;
class TrackHeaderView;
class RegionSequenceView;
class PlaybackRegionView;

//==============================================================================
/**
 DocumentView Class -
    This class provides basic foundation to show the ARA Document as well as
    their current selection state

    currently, even though it is developed under Celemony's repository it's
    a non-suprvised branch to be reviewed and decided what to do with later.
 
 Goals (once finished):
    - become part of ARA or JUCE-ARA SDK
    - provide juce::LookAndFeel mechanism so it could be customized for developer needs.
    - become a baseclass to use as view component for most ARA-JUCE based products..
 TODOs:
    - configuration for all sizes: track height, ruler height, track header width etc.
    - a setting to make track header width optionally be resizable by user
    - refactor RulersViews to have RulersView::RulerBase and subclasses.
    - option to show regions including their head and tail (for crossfades mostly, renderer will already provide proper samples but time ranges must be adjusted for this and updated if head/tail change)
    - replace Viewport with better mechanism to avoid overflow with long documents and high zoom level.
 
 */
class DocumentView  : public AudioProcessorEditor,
                      public AudioProcessorEditorARAExtension,
                      private ARAEditorView::Listener,
                      private ARADocument::Listener,
                      private juce::Timer
{
public:
    DocumentView (AudioProcessor&);
    ~DocumentView();

    /*
     Creates a new PlaybackRegionView which will be owned.
     This allows customizing PlaybackRegionView Component to desired behavior.
     (for example: showing notes)
     */
    virtual PlaybackRegionView* createViewForPlaybackRegion (ARAPlaybackRegion*);

    /*
     Creates a new RegionSequenceView which will be owned.
     This allows customizing RegionSequenceView Component to desired behavior.
     (for example: allow showing cross-fades or interaction between regions)
     */
    virtual RegionSequenceView* createViewForRegionSequence (ARARegionSequence*);

    /*
     Creates a new TrackHeaderView which will be owned.
     This allows customizing TrackHeaderView Component to desired behavior.
     */
    virtual TrackHeaderView* createHeaderViewForRegionSequence (ARARegionSequence*);

    // total time range
    Range<double> getTimeRange() const { return timeRange; }

    // currently visible time range
    Range<double> getVisibleTimeRange() const;
// TODO JUCE_ARA if we want to make this into a reusable view, then zooming should use this primitive:
//  void setVisibleTimeRange (double start, double end);
//  It would limit the new visibile range to getTimeRange(), trying to keep requested duration unchanged.
//  Another method zoomBy(float factor) can be added on top of this, which deals with keeping the relative
//  playhead positon unchanged if it is visible while zooming, otherwise keeps current view centered.
//  This will be easy to do since it is all in linear time now.

    // convert between time and x coordinate
    int getPlaybackRegionsViewsXForTime (double time) const;
    double getPlaybackRegionsViewsTimeForX (int x) const;

    // flag that our view needs to be rebuilt
    void invalidateRegionSequenceViews();

    Component& getPlaybackRegionsView() { return playbackRegionsView; }
    Component& getTrackHeadersView() { return trackHeadersView; }
    Viewport& getTrackHeadersViewPort() { return trackHeadersViewPort; }
    Viewport& getRulersViewPort() { return rulersViewPort; }

    AudioFormatManager& getAudioFormatManger() { return audioFormatManger; }

    /*
     Sets a juce::AudioPlayHead::CurrentPositionInfo pointer that
     should be used to show playhead.
     Note: CurrentPositionInfo is only valid within processBlock calls,
           The object should be updated only within audio thread.
     */
    void setCurrentPositionInfo (const AudioPlayHead::CurrentPositionInfo*);

    double getPlayheadTimePosition() const { return playheadTimePosition; }

    void setShowOnlySelectedRegionSequences (bool newVal);
    bool isShowingOnlySelectedRegionSequences() { return showOnlySelectedRegionSequences; }

    void setIsRulersVisible (bool shouldBeVisible);
    bool isRulersVisible() const { return rulersViewPort.isVisible(); }

    // DocumentView States
    void setScrollFollowsPlaybackState (bool followPlayhead) { shouldFollowPlayhead.setValue (followPlayhead); }
    bool getScrollFollowPlaybackState() const { return shouldFollowPlayhead.getValue(); }
    juce::Value& getScrollFollowsPlaybackStateValue() { return shouldFollowPlayhead; }

    double getPixelsPerSecond() const { return pixelsPerSecond; }
    void setPixelsPerSecond (double newValue);
    int getTrackHeight() const { return trackHeight; }
    void setTrackHeight (int newHeight);
    bool isMaximumPixelsPerSecond() const { return pixelsPerSecond > minPixelsPerSecond; }
    bool isMinimumPixelsPerSecond() const { return pixelsPerSecond < maxPixelsPerSecond; }

    //==============================================================================
    void parentHierarchyChanged() override;
    void paint (Graphics&) override;
    void resized() override;

    // juce::Timer overrides
    void timerCallback() override;

    // ARAEditorView::Listener overrides
    void onHideRegionSequences (std::vector<ARARegionSequence*> const& regionSequences) override;

    // ARADocument::Listener overrides
    void didEndEditing (ARADocument* document) override;
    void didAddRegionSequenceToDocument (ARADocument* document, ARARegionSequence* regionSequence) override;
    void didReorderRegionSequencesInDocument (ARADocument* document) override;

    //==============================================================================
    /**
     A class for receiving events from a DocumentView.

     You can register a DocumentView::Listener with a DocumentView using DocumentView::addListener()
     method, and it will be called on changes.

     @see DocumentView::addListener, DocumentView::removeListener
     */
    class Listener
    {
    public:
        /** Destructor. */
        virtual ~Listener() {}

        /** Called when a DocumentView selection is changed.
            This can happen when scrolled or zoomed/scaled on the horizontal axis.

         @param newRangeStartInSeconds    the new range start of document's selection.
         @param newRangeEndInSeconds      the new range end of document's selection.
         @param pixelsPerSecond           current pixels per second.
         */
        virtual void timelineSelectionChanged (double newRangeStartInSeconds, double newRangeEndInSeconds, double pixelsPerSecond) = 0;
        /** Called when a trackHeight is changed.

         @param newTrackHeight           new trackHeight in pixels.
         */
        virtual void trackHeightChanged (int newTrackHeight) {};
        /** Called when a rulersHeight is changed.

         @param newRulersHeight           new rulersHeight in pixels.
         */
        virtual void rulersHeightChanged (int newRulersHeight) {};
    };

    /** Registers a listener that will be called for changes of the DocumentView. */
    void addListener (Listener* listener);

    /** Deregisters a previously-registered listener. */
    void removeListener (Listener* listener);

private:
    void rebuildRegionSequenceViews();
    void updatePlayheadBounds();

private:
    // simple utility class to show playhead position
    class PlayheadView : public Component
    {
    public:
        PlayheadView (DocumentView& documentView);
        void paint (Graphics&) override;
    private:
        DocumentView& documentView;
    };

    // simple utility class to partially sync scroll postions of our view ports
    class ScrollMasterViewPort : public Viewport
    {
    public:
        ScrollMasterViewPort (DocumentView& documentView) : documentView (documentView) {};
        void visibleAreaChanged (const Rectangle<int>& newVisibleArea) override;
    private:
        DocumentView& documentView;
    };

    OwnedArray<RegionSequenceView> regionSequenceViews;

    ScrollMasterViewPort playbackRegionsViewPort;
    Component playbackRegionsView;
    PlayheadView playheadView;
    Viewport trackHeadersViewPort;
    Component trackHeadersView;
    Viewport rulersViewPort;
    std::unique_ptr<RulersView> rulersView;

    AudioFormatManager audioFormatManger;

    // Component View States
    Value shouldFollowPlayhead;
    double pixelsPerSecond;
    int trackHeight;
    double maxPixelsPerSecond, minPixelsPerSecond;

    bool regionSequenceViewsAreInvalid = true;
    bool showOnlySelectedRegionSequences = false;
    Range<double> timeRange;
    double playheadTimePosition = 0.0;
    
    const juce::AudioPlayHead::CurrentPositionInfo* positionInfoPtr;
    ListenerList<Listener> listeners;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DocumentView)
};
