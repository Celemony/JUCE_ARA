/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "DocumentView.h"

//==============================================================================
/**
    Editor class for ARAPluginDemo
*/
class ARAPluginDemoAudioProcessorEditor   : public juce::AudioProcessorEditor,
                                            public juce::AudioProcessorEditorARAExtension,
                                            private juce::Timer
{
public:
    ARAPluginDemoAudioProcessorEditor (ARAPluginDemoAudioProcessor&);
    ~ARAPluginDemoAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    // juce::Timer
    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ARAPluginDemoAudioProcessor& audioProcessor;

    std::unique_ptr<DocumentView> documentView;

    juce::TooltipWindow tooltip;
    juce::TextButton onlySelectedTracksButton;
    juce::TextButton followPlayHeadButton;
    juce::Label playheadLinearPositionLabel, playheadMusicalPositionLabel;
    juce::TextButton horizontalZoomInButton, horizontalZoomOutButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ARAPluginDemoAudioProcessorEditor)
};
