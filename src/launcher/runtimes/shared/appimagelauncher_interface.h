#pragma once

/**
 * Tries to forward the execution of the AppImage pointed by <appImagePath> the the integration assistant. This could
 * be used to show first time run dialogs.
 *
 * @param appImagePath
 * @param args
 * @return 0 if the integration assistant was properly executed, 1 otherwise.
 */
int tryForwardExecToIntegrationAssistant(int argc, char* const* argv, char* appImagePath);

/**
 * Checks whether the integration assistant should be used to open <target>.
 *
 * The integration assistant will not be used in the following scenarios:
 * - APPIMAGELAUNCHER_DISABLE is set
 * - the appimage-services AppImage is being opened
 * - the target is inside another AppImage
 * @return 1 (true) if the integration assistant is enabled. 0 (false) otherwise
 */
int shouldIntegrationAssistantBeUsedOn(const char* target);

