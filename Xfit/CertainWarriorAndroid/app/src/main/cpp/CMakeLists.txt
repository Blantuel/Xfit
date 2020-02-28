cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_C_COMPILER, "-std=c11")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_STANDARD,"-std=c++1z")

set(XfitPath "../../../../../Xfit/")

set(CertainWarriorPath "../../../../../CertainWarrior/")

set(FreetypePath "../../../../../Freetype/src/")
set(LibjpegPath "../../../../../Libjpeg/")
set(LiboggPath "../../../../../Libogg/")
set(LibpngPath "../../../../../Libpng/")
set(ZlibPath "../../../../../Zlib/")

add_library(Freetype STATIC ${FreetypePath}autofit/autofit.c ${FreetypePath}bdf/bdf.c ${FreetypePath}cff/cff.c ${FreetypePath}base/ftbase.c ${FreetypePath}base/ftbbox.c ${FreetypePath}base/ftbdf.c
${FreetypePath}base/ftbitmap.c ${FreetypePath}cache/ftcache.c ${FreetypePath}base/ftcid.c ${FreetypePath}base/ftfstype.c ${FreetypePath}base/ftgasp.c ${FreetypePath}base/ftglyph.c
${FreetypePath}base/ftgxval.c  ${FreetypePath}gzip/ftgzip.c ${FreetypePath}base/ftinit.c ${FreetypePath}lzw/ftlzw.c ${FreetypePath}base/ftmm.c ${FreetypePath}base/ftotval.c ${FreetypePath}base/ftpatent.c
${FreetypePath}base/ftpfr.c ${FreetypePath}base/ftstroke.c ${FreetypePath}base/ftsynth.c ${FreetypePath}base/ftsystem.c ${FreetypePath}base/fttype1.c ${FreetypePath}base/ftwinfnt.c ${FreetypePath}pcf/pcf.c
${FreetypePath}pfr/pfr.c  ${FreetypePath}psaux/psaux.c  ${FreetypePath}pshinter/pshinter.c ${FreetypePath}psnames/psmodule.c ${FreetypePath}raster/raster.c ${FreetypePath}sfnt/sfnt.c ${FreetypePath}smooth/smooth.c
${FreetypePath}truetype/truetype.c  ${FreetypePath}type1/type1.c ${FreetypePath}cid/type1cid.c ${FreetypePath}type42/type42.c ${FreetypePath}winfonts/winfnt.c ${FreetypePath}base/ftobjs.c)

target_compile_definitions(Freetype PUBLIC FT2_BUILD_LIBRARY)

add_library(Libjpeg STATIC ${LibjpegPath}jcapimin.c ${LibjpegPath}jcapistd.c ${LibjpegPath}jccoefct.c ${LibjpegPath}jccolor.c ${LibjpegPath}jcdctmgr.c ${LibjpegPath}jchuff.c
 ${LibjpegPath}jcinit.c ${LibjpegPath}jcmainct.c ${LibjpegPath}jcmarker.c ${LibjpegPath}jcmaster.c ${LibjpegPath}jcomapi.c ${LibjpegPath}jcparam.c
 ${LibjpegPath}jcprepct.c ${LibjpegPath}jcsample.c ${LibjpegPath}jctrans.c ${LibjpegPath}jdapimin.c ${LibjpegPath}jdapistd.c
 ${LibjpegPath}jdatadst.c ${LibjpegPath}jdatasrc.c ${LibjpegPath}jdcoefct.c ${LibjpegPath}jdcolor.c ${LibjpegPath}jddctmgr.c
 ${LibjpegPath}jdinput.c ${LibjpegPath}jdmainct.c ${LibjpegPath}jdmarker.c ${LibjpegPath}jdmaster.c ${LibjpegPath}jdmerge.c
 ${LibjpegPath}jdpostct.c ${LibjpegPath}jdsample.c ${LibjpegPath}jdtrans.c ${LibjpegPath}jerror.c ${LibjpegPath}jfdctflt.c ${LibjpegPath}jfdctfst.c
 ${LibjpegPath}jfdctint.c ${LibjpegPath}jidctflt.c ${LibjpegPath}jidctfst.c ${LibjpegPath}jidctint.c ${LibjpegPath}jquant1.c
 ${LibjpegPath}jquant2.c ${LibjpegPath}jutils.c ${LibjpegPath}jmemmgr.c)

add_library(Libogg STATIC ${LiboggPath}analysis.c ${LiboggPath}barkmel.c ${LiboggPath}bitrate.c ${LiboggPath}bitwise.c
${LiboggPath}block.c ${LiboggPath}codebook.c ${LiboggPath}envelope.c ${LiboggPath}floor0.c
${LiboggPath}floor1.c ${LiboggPath}framing.c ${LiboggPath}info.c ${LiboggPath}lookup.c
${LiboggPath}lpc.c ${LiboggPath}lsp.c ${LiboggPath}mapping0.c ${LiboggPath}mdct.c
${LiboggPath}psy.c ${LiboggPath}registry.c ${LiboggPath}res0.c ${LiboggPath}sharedbook.c
${LiboggPath}smallft.c ${LiboggPath}synthesis.c ${LiboggPath}tone.c ${LiboggPath}vorbisenc.c
${LiboggPath}vorbisfile.c ${LiboggPath}window.c)

add_library(Libpng STATIC ${LibpngPath}png.c ${LibpngPath}pngerror.c ${LibpngPath}pngget.c ${LibpngPath}pngmem.c ${LibpngPath}pngpread.c ${LibpngPath}pngread.c ${LibpngPath}pngrio.c ${LibpngPath}pngrtran.c ${LibpngPath}pngrutil.c
${LibpngPath}pngset.c ${LibpngPath}pngtrans.c ${LibpngPath}pngwio.c ${LibpngPath}pngwrite.c ${LibpngPath}pngwtran.c ${LibpngPath}pngwutil.c)

add_library(Zlib STATIC ${ZlibPath}adler32.c ${ZlibPath}compress.c ${ZlibPath}crc32.c ${ZlibPath}deflate.c ${ZlibPath}gzclose.c ${ZlibPath}gzlib.c
 ${ZlibPath}gzread.c ${ZlibPath}gzwrite.c ${ZlibPath}infback.c ${ZlibPath}inffast.c ${ZlibPath}inflate.c ${ZlibPath}inftrees.c ${ZlibPath}trees.c ${ZlibPath}uncompr.c ${ZlibPath}zutil.c)


add_library(Xfit STATIC ${XfitPath}_system/_Android.cpp ${XfitPath}_system/_Loop.cpp ${XfitPath}_system/_OpenGL.cpp ${XfitPath}_system/_Vulkan.cpp

        ${XfitPath}component/Button.cpp ${XfitPath}component/CheckBox.cpp ${XfitPath}component/FadeLabelButton.cpp ${XfitPath}component/LabelButton.cpp ${XfitPath}component/LabelListButton.cpp
        ${XfitPath}component/LabelOptionButton.cpp ${XfitPath}component/LabelToggleButton.cpp ${XfitPath}component/RadioButton.cpp ${XfitPath}component/Scrollbar.cpp ${XfitPath}component/SizeButton.cpp
        ${XfitPath}component/Slider.cpp  ${XfitPath}component/EditBox.cpp

        ${XfitPath}data/Compress.cpp  ${XfitPath}data/ImageUtil.cpp

        ${XfitPath}decoder/Decoder.cpp  ${XfitPath}decoder/ImageDecoder.cpp  ${XfitPath}decoder/JPEGDecoder.cpp ${XfitPath}decoder/OGGDecoder.cpp ${XfitPath}decoder/PNGDecoder.cpp
        ${XfitPath}decoder/SoundDecoder.cpp ${XfitPath}decoder/WAVDecoder.cpp #[[${XfitPath}decoder/WebPAnimateDecoder.cpp ${XfitPath}decoder/WebPDecoder.cpp]]

        ${XfitPath}effect/Blend.cpp  ${XfitPath}effect/Filter.cpp

        ${XfitPath}encoder/Encoder.cpp  ${XfitPath}encoder/ImageEncoder.cpp #[[${XfitPath}encoder/WebPAnimateLosslessEncoder.cpp  ${XfitPath}encoder/WebPLosslessEncoder.cpp]]

        ${XfitPath}file/ResourceManager.cpp

        ${XfitPath}math/Hexagon.cpp ${XfitPath}math/Lines.cpp ${XfitPath}math/Matrix.cpp

        ${XfitPath}network/Socket.cpp ${XfitPath}network/IP.cpp

        ${XfitPath}object/AnimateMultiInstance.cpp ${XfitPath}object/AnimateObject.cpp ${XfitPath}object/Image.cpp ${XfitPath}object/ImageBase.cpp ${XfitPath}object/ImageInstance.cpp
        ${XfitPath}object/ImageMultiInstance.cpp ${XfitPath}object/LabelImage ${XfitPath}object/Line.cpp ${XfitPath}object/LineInstance.cpp ${XfitPath}object/MatrixObject.cpp ${XfitPath}object/Object.cpp
        ${XfitPath}object/RTAnimateObject.cpp ${XfitPath}object/RTAnimateObjectBase.cpp ${XfitPath}object/ScaleHeader.cpp ${XfitPath}object/ScaleImage.cpp ${XfitPath}object/Shape.cpp ${XfitPath}object/ShapeInstance
        ${XfitPath}object/TextBoxImage.cpp

        ${XfitPath}object3D/Image3D.cpp ${XfitPath}object3D/Object3D.cpp ${XfitPath}object3D/Shape3D.cpp

        ${XfitPath}physics/Collision.cpp ${XfitPath}physics/RectHitTest.cpp ${XfitPath}physics/SizeRectHitTest.cpp

        ${XfitPath}resource/AnimateBase.cpp ${XfitPath}resource/AnimateFrame.cpp ${XfitPath}resource/Frame.cpp ${XfitPath}resource/FrameArray.cpp ${XfitPath}resource/Index.cpp ${XfitPath}resource/RTAnimateFrame.cpp
        ${XfitPath}resource/Sampler.cpp  ${XfitPath}resource/ShapeVertex.cpp ${XfitPath}resource/Vertex.cpp

        ${XfitPath}sound/Sound.cpp

        ${XfitPath}system/Context.cpp ${XfitPath}system/Input.cpp ${XfitPath}system/Screen.cpp ${XfitPath}system/System.cpp

        ${XfitPath}text/Font.cpp ${XfitPath}text/Label.cpp ${XfitPath}text/SizeAnimateTextBox.cpp ${XfitPath}text/SizeLabel.cpp ${XfitPath}text/SizeTextBox.cpp ${XfitPath}text/Text.cpp ${XfitPath}text/Textbox.cpp

        ${XfitPath}time/Time.cpp

        ${XfitPath}view/Viewport.cpp)

add_library(CertainWarrior SHARED ${CertainWarriorPath}Action.cpp ${CertainWarriorPath}ActionCard.cpp ${CertainWarriorPath}ActionCardItem.cpp ${CertainWarriorPath}Ambush.cpp ${CertainWarriorPath}Armor.cpp
        ${CertainWarriorPath}Attack.cpp ${CertainWarriorPath}ChatScene.cpp ${CertainWarriorPath}ConnectScene.cpp ${CertainWarriorPath}Dodge.cpp ${CertainWarriorPath}FightScene.cpp
        ${CertainWarriorPath}Focus.cpp ${CertainWarriorPath}FocusAttack.cpp ${CertainWarriorPath}Guard.cpp ${CertainWarriorPath}HoldOut.cpp ${CertainWarriorPath}JewelryBar.cpp ${CertainWarriorPath}Mage.cpp
        ${CertainWarriorPath}MageAction.cpp ${CertainWarriorPath}MageAttack.cpp ${CertainWarriorPath}main.cpp ${CertainWarriorPath}Option.cpp ${CertainWarriorPath}Patience.cpp ${CertainWarriorPath}PoisonedNeedle.cpp
        ${CertainWarriorPath}Rest.cpp ${CertainWarriorPath}Smoke.cpp ${CertainWarriorPath}SmokeBomb.cpp ${CertainWarriorPath}StatBar ${CertainWarriorPath}Thief.cpp ${CertainWarriorPath}ThiefStat.cpp
        ${CertainWarriorPath}ThiefWeapon.cpp ${CertainWarriorPath}ThrowWeapon.cpp ${CertainWarriorPath}Title.cpp ${CertainWarriorPath}Unit.cpp ${CertainWarriorPath}UnlockAction.cpp ${CertainWarriorPath}VisualTextBox.cpp
        ${CertainWarriorPath}Warrior.cpp ${CertainWarriorPath}WarriorStat.cpp ${CertainWarriorPath}Weapon.cpp)

target_include_directories(Freetype PUBLIC ${FreetypePath}../include)

target_include_directories(Xfit PUBLIC ${XfitPath}../Freetype/include  ${XfitPath}../WebP/include)

target_include_directories(CertainWarrior PUBLIC ${CertainWarriorPath}../Xfit)

add_library(native_app_glue STATIC
        ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

target_include_directories(Xfit PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue)

target_include_directories(CertainWarrior PRIVATE
        ${ANDROID_NDK}/sources/android/native_app_glue)

set(CMAKE_SHARED_LINKER_FLAGS
        "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

target_link_libraries(CertainWarrior android native_app_glue EGL GLESv3 OpenSLES z log Xfit Freetype Libjpeg Libogg Libpng Zlib )
