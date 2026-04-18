#!/bin/bash
# Creates a macOS .pkg installer for the Lupo AU plugin + factory presets.
# Requirements: Xcode command line tools (pkgbuild, productbuild)
# Usage: ./create_installer.sh [version]

set -euo pipefail

VERSION="${1:-1.0.0}"
PLUGIN_NAME="Lupo"
MANUFACTURER="Matthias Pueski"
BUNDLE_ID="com.MatthiasPueski.Lupo"

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/Builds/MacOSX/build/Release"
PRESET_SRC="$HOME/Library/Audio/Presets/pueski/Lupo"
INSTALLER_DIR="$SCRIPT_DIR/Installer"
STAGING_DIR="$INSTALLER_DIR/staging"
PKG_DIR="$INSTALLER_DIR/packages"
OUTPUT_DIR="$INSTALLER_DIR/output"

echo "==> Building Lupo $VERSION installer"

# ── Sanity checks ────────────────────────────────────────────────────────────
if [ ! -d "$BUILD_DIR/$PLUGIN_NAME.component" ]; then
  echo "ERROR: $BUILD_DIR/$PLUGIN_NAME.component not found."
  echo "       Build the Release target in Xcode first."
  exit 1
fi

# ── Clean staging area ───────────────────────────────────────────────────────
rm -rf "$STAGING_DIR" "$PKG_DIR"
mkdir -p "$PKG_DIR" "$OUTPUT_DIR"

# ── Package 1: AU Component ──────────────────────────────────────────────────
echo "--> Staging AU component"
AU_ROOT="$STAGING_DIR/au"
mkdir -p "$AU_ROOT/Library/Audio/Plug-Ins/Components"
cp -R "$BUILD_DIR/$PLUGIN_NAME.component" \
      "$AU_ROOT/Library/Audio/Plug-Ins/Components/"

pkgbuild \
  --root        "$AU_ROOT" \
  --identifier  "$BUNDLE_ID.component" \
  --version     "$VERSION" \
  --install-location "/" \
  "$PKG_DIR/LupoComponent.pkg"

echo "    -> $PKG_DIR/LupoComponent.pkg"

# ── Package 2: Factory Presets ───────────────────────────────────────────────
echo "--> Staging factory presets"
PRESET_ROOT="$STAGING_DIR/presets"
PRESET_DEST="$PRESET_ROOT/Library/Audio/Presets/$MANUFACTURER/$PLUGIN_NAME"
mkdir -p "$PRESET_DEST"

if [ -d "$PRESET_SRC" ]; then
  # Copy only the canonical preset files (xml + companion files)
  rsync -a --exclude="midilearn.cfg" "$PRESET_SRC/" "$PRESET_DEST/"
  echo "    -> copied presets from $PRESET_SRC"
else
  echo "    WARNING: Preset source not found at $PRESET_SRC – skipping preset package."
fi

pkgbuild \
  --root        "$PRESET_ROOT" \
  --identifier  "$BUNDLE_ID.presets" \
  --version     "$VERSION" \
  --install-location "/" \
  "$PKG_DIR/LupoPresets.pkg"

echo "    -> $PKG_DIR/LupoPresets.pkg"

# ── Distribution XML ─────────────────────────────────────────────────────────
DIST_XML="$INSTALLER_DIR/distribution.xml"
cat > "$DIST_XML" <<DISTEOF
<?xml version="1.0" encoding="utf-8"?>
<installer-gui-script minSpecVersion="2">

    <title>Lupo Synthesizer $VERSION</title>
    <welcome    file="welcome.html"    mime-type="text/html" />
    <license    file="license.html"    mime-type="text/html" />
    <readme     file="readme.html"     mime-type="text/html" />

    <options customize="allow" require-scripts="false" rootVolumeOnly="false"/>

    <choices-outline>
        <line choice="LupoComponent"/>
        <line choice="LupoPresets"/>
    </choices-outline>

    <choice id="LupoComponent"
            title="Lupo AU Plugin"
            description="Installs the Lupo Audio Unit plugin to /Library/Audio/Plug-Ins/Components."
            start_selected="true"
            start_enabled="true"
            start_visible="true">
        <pkg-ref id="$BUNDLE_ID.component"/>
    </choice>

    <choice id="LupoPresets"
            title="Factory Presets"
            description="Installs factory presets to /Library/Audio/Presets/$MANUFACTURER/Lupo."
            start_selected="true"
            start_enabled="true"
            start_visible="true">
        <pkg-ref id="$BUNDLE_ID.presets"/>
    </choice>

    <pkg-ref id="$BUNDLE_ID.component" version="$VERSION" onConclusion="none">LupoComponent.pkg</pkg-ref>
    <pkg-ref id="$BUNDLE_ID.presets"   version="$VERSION" onConclusion="none">LupoPresets.pkg</pkg-ref>

</installer-gui-script>
DISTEOF

# ── Stub HTML resources (replace with real ones if available) ────────────────
RESOURCES_DIR="$INSTALLER_DIR/resources"
mkdir -p "$RESOURCES_DIR"

for f in welcome license readme; do
  DEST="$RESOURCES_DIR/$f.html"
  [ -f "$DEST" ] && continue           # Don't overwrite if user has real files

  case "$f" in
    welcome)
      cat > "$DEST" <<HTML
<html><body>
<h2>Welcome to Lupo $VERSION</h2>
<p>This installer will install the <strong>Lupo</strong> synthesizer Audio Unit
plugin by $MANUFACTURER.</p>
<p>Lupo is a polyphonic, multi-oscillator synthesizer with filters, effects,
arpeggiator and step sequencer.</p>
</body></html>
HTML
      ;;
    license)
      cat > "$DEST" <<HTML
<html><body>
<h2>License</h2>
<p>Copyright &copy; $(date +%Y) $MANUFACTURER. All rights reserved.</p>
<p>This software is provided "as is", without warranty of any kind.</p>
</body></html>
HTML
      ;;
    readme)
      cat > "$DEST" <<HTML
<html><body>
<h2>Lupo $VERSION – Read Me</h2>
<p>After installation, open your DAW and rescan Audio Units.
Lupo will appear under <em>$MANUFACTURER</em>.</p>
<p>Factory presets are installed to:<br/>
<code>/Library/Audio/Presets/$MANUFACTURER/Lupo</code></p>
</body></html>
HTML
      ;;
  esac
done

# ── Final product package ────────────────────────────────────────────────────
OUTPUT_PKG="$OUTPUT_DIR/Lupo-$VERSION.pkg"

echo "--> Building final installer: $OUTPUT_PKG"
productbuild \
  --distribution    "$DIST_XML" \
  --resources       "$RESOURCES_DIR" \
  --package-path    "$PKG_DIR" \
  "$OUTPUT_PKG"

echo ""
echo "==> Done: $OUTPUT_PKG"
echo ""
echo "    Install with:  open \"$OUTPUT_PKG\""
echo "    Or:            sudo installer -pkg \"$OUTPUT_PKG\" -target /"
