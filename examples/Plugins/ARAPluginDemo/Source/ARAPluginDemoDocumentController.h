#pragma once

#include <juce_audio_plugin_client/juce_audio_plugin_client.h>

//==============================================================================
// The document controller is the central point of communication between the
// ARA host and our plug-in. It also serves as factory for any custom subclass
// in the ARA model graph or for instance roles implementations.
// In this example we're only customizing the audio modification in the graph
// and the playback renderer instance role.
// To ensure proper persistency of our overridden audio modification, we also
// must customize the store/restore code.
class ARAPluginDemoDocumentController     : public juce::ARADocumentControllerSpecialisation
{
public:
    //==============================================================================
    using ARADocumentControllerSpecialisation::ARADocumentControllerSpecialisation;

protected:
    //==============================================================================
    // Override document controller customization methods here

    juce::ARAAudioModification* doCreateAudioModification (juce::ARAAudioSource* audioSource, ARA::ARAAudioModificationHostRef hostRef, const juce::ARAAudioModification* optionalModificationToClone) noexcept override;

    juce::ARAPlaybackRenderer* doCreatePlaybackRenderer() noexcept override;

    bool doRestoreObjectsFromStream (juce::ARAInputStream& input, const juce::ARARestoreObjectsFilter* filter) noexcept override;
    bool doStoreObjectsToStream (juce::ARAOutputStream& output, const juce::ARAStoreObjectsFilter* filter) noexcept override;

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ARAPluginDemoDocumentController)
};
