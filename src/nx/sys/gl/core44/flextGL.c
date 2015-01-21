/* WARNING: This file was automatically generated */
/* Do not edit. */

#include "flextGL.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(WIN32)
#define FLEXT_CALL __stdcall
#else
#define FLEXT_CALL
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* FLEXT_C_EXTENSION
 * Compiling in strict c leads to errors due to void* to function ptr
 * cast being illegal. Mark as extension so that the compiler will
 * accept it.
 */
#if defined(__GNUC__) || defined(__clang__)
#define FLEXT_C_EXTENSION(x) __extension__(x)
#else
#define FLEXT_C_EXTENSION(x) x
#endif

typedef void(FLEXT_CALL *GLPROC)();

void flextLoadOpenGLFunctions(void);

static void open_libgl(void);
static void close_libgl(void);
static GLPROC get_proc(const char *proc);
static void add_extension(const char* extension);

int flextInit(void)
{
    GLint minor, major;
    GLint num_extensions;
    int i;

    open_libgl();
    flextLoadOpenGLFunctions();
    close_libgl();

    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    /* --- Check for minimal version and profile --- */

    if (major * 10 + minor < 44) {
#if !defined(FLEXT_NO_LOGGING)
        fprintf(stderr, "Error: OpenGL version 4.4 not supported.\n");
        fprintf(stderr, "       Your version is %d.%d.\n", major, minor);
        fprintf(stderr, "       Try updating your graphics driver.\n");
#endif
        return GL_FALSE;
    }


    /* --- Check for extensions --- */

    glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);

    for (i = 0; i < num_extensions; i++) {
        add_extension((const char*)glGetStringi(GL_EXTENSIONS, i));
    }

    if (!FLEXT_ARB_direct_state_access) {
        fprintf(stderr, "Error: OpenGL extension GL_ARB_direct_state_access not supported.\n");
        fprintf(stderr, "       Try updating your graphics driver.\n");
        return GL_FALSE;
    }

    return GL_TRUE;
}



void flextLoadOpenGLFunctions(void)
{
    /* --- Function pointer loading --- */


    /* GL_ARB_bindless_texture */

    glpfGetImageHandleARB = (PFNGLGETIMAGEHANDLEARB_PROC*)get_proc("glGetImageHandleARB");
    glpfGetTextureHandleARB = (PFNGLGETTEXTUREHANDLEARB_PROC*)get_proc("glGetTextureHandleARB");
    glpfGetTextureSamplerHandleARB = (PFNGLGETTEXTURESAMPLERHANDLEARB_PROC*)get_proc("glGetTextureSamplerHandleARB");
    glpfGetVertexAttribLui64vARB = (PFNGLGETVERTEXATTRIBLUI64VARB_PROC*)get_proc("glGetVertexAttribLui64vARB");
    glpfIsImageHandleResidentARB = (PFNGLISIMAGEHANDLERESIDENTARB_PROC*)get_proc("glIsImageHandleResidentARB");
    glpfIsTextureHandleResidentARB = (PFNGLISTEXTUREHANDLERESIDENTARB_PROC*)get_proc("glIsTextureHandleResidentARB");
    glpfMakeImageHandleNonResidentARB = (PFNGLMAKEIMAGEHANDLENONRESIDENTARB_PROC*)get_proc("glMakeImageHandleNonResidentARB");
    glpfMakeImageHandleResidentARB = (PFNGLMAKEIMAGEHANDLERESIDENTARB_PROC*)get_proc("glMakeImageHandleResidentARB");
    glpfMakeTextureHandleNonResidentARB = (PFNGLMAKETEXTUREHANDLENONRESIDENTARB_PROC*)get_proc("glMakeTextureHandleNonResidentARB");
    glpfMakeTextureHandleResidentARB = (PFNGLMAKETEXTUREHANDLERESIDENTARB_PROC*)get_proc("glMakeTextureHandleResidentARB");
    glpfProgramUniformHandleui64ARB = (PFNGLPROGRAMUNIFORMHANDLEUI64ARB_PROC*)get_proc("glProgramUniformHandleui64ARB");
    glpfProgramUniformHandleui64vARB = (PFNGLPROGRAMUNIFORMHANDLEUI64VARB_PROC*)get_proc("glProgramUniformHandleui64vARB");
    glpfUniformHandleui64ARB = (PFNGLUNIFORMHANDLEUI64ARB_PROC*)get_proc("glUniformHandleui64ARB");
    glpfUniformHandleui64vARB = (PFNGLUNIFORMHANDLEUI64VARB_PROC*)get_proc("glUniformHandleui64vARB");
    glpfVertexAttribL1ui64ARB = (PFNGLVERTEXATTRIBL1UI64ARB_PROC*)get_proc("glVertexAttribL1ui64ARB");
    glpfVertexAttribL1ui64vARB = (PFNGLVERTEXATTRIBL1UI64VARB_PROC*)get_proc("glVertexAttribL1ui64vARB");


    /* GL_ARB_direct_state_access */

    glpfBindTextureUnit = (PFNGLBINDTEXTUREUNIT_PROC*)get_proc("glBindTextureUnit");
    glpfBlitNamedFramebuffer = (PFNGLBLITNAMEDFRAMEBUFFER_PROC*)get_proc("glBlitNamedFramebuffer");
    glpfCheckNamedFramebufferStatus = (PFNGLCHECKNAMEDFRAMEBUFFERSTATUS_PROC*)get_proc("glCheckNamedFramebufferStatus");
    glpfClearNamedBufferData = (PFNGLCLEARNAMEDBUFFERDATA_PROC*)get_proc("glClearNamedBufferData");
    glpfClearNamedBufferSubData = (PFNGLCLEARNAMEDBUFFERSUBDATA_PROC*)get_proc("glClearNamedBufferSubData");
    glpfClearNamedFramebufferfi = (PFNGLCLEARNAMEDFRAMEBUFFERFI_PROC*)get_proc("glClearNamedFramebufferfi");
    glpfClearNamedFramebufferfv = (PFNGLCLEARNAMEDFRAMEBUFFERFV_PROC*)get_proc("glClearNamedFramebufferfv");
    glpfClearNamedFramebufferiv = (PFNGLCLEARNAMEDFRAMEBUFFERIV_PROC*)get_proc("glClearNamedFramebufferiv");
    glpfClearNamedFramebufferuiv = (PFNGLCLEARNAMEDFRAMEBUFFERUIV_PROC*)get_proc("glClearNamedFramebufferuiv");
    glpfCompressedTextureSubImage1D = (PFNGLCOMPRESSEDTEXTURESUBIMAGE1D_PROC*)get_proc("glCompressedTextureSubImage1D");
    glpfCompressedTextureSubImage2D = (PFNGLCOMPRESSEDTEXTURESUBIMAGE2D_PROC*)get_proc("glCompressedTextureSubImage2D");
    glpfCompressedTextureSubImage3D = (PFNGLCOMPRESSEDTEXTURESUBIMAGE3D_PROC*)get_proc("glCompressedTextureSubImage3D");
    glpfCopyNamedBufferSubData = (PFNGLCOPYNAMEDBUFFERSUBDATA_PROC*)get_proc("glCopyNamedBufferSubData");
    glpfCopyTextureSubImage1D = (PFNGLCOPYTEXTURESUBIMAGE1D_PROC*)get_proc("glCopyTextureSubImage1D");
    glpfCopyTextureSubImage2D = (PFNGLCOPYTEXTURESUBIMAGE2D_PROC*)get_proc("glCopyTextureSubImage2D");
    glpfCopyTextureSubImage3D = (PFNGLCOPYTEXTURESUBIMAGE3D_PROC*)get_proc("glCopyTextureSubImage3D");
    glpfCreateBuffers = (PFNGLCREATEBUFFERS_PROC*)get_proc("glCreateBuffers");
    glpfCreateFramebuffers = (PFNGLCREATEFRAMEBUFFERS_PROC*)get_proc("glCreateFramebuffers");
    glpfCreateProgramPipelines = (PFNGLCREATEPROGRAMPIPELINES_PROC*)get_proc("glCreateProgramPipelines");
    glpfCreateQueries = (PFNGLCREATEQUERIES_PROC*)get_proc("glCreateQueries");
    glpfCreateRenderbuffers = (PFNGLCREATERENDERBUFFERS_PROC*)get_proc("glCreateRenderbuffers");
    glpfCreateSamplers = (PFNGLCREATESAMPLERS_PROC*)get_proc("glCreateSamplers");
    glpfCreateTextures = (PFNGLCREATETEXTURES_PROC*)get_proc("glCreateTextures");
    glpfCreateTransformFeedbacks = (PFNGLCREATETRANSFORMFEEDBACKS_PROC*)get_proc("glCreateTransformFeedbacks");
    glpfCreateVertexArrays = (PFNGLCREATEVERTEXARRAYS_PROC*)get_proc("glCreateVertexArrays");
    glpfDisableVertexArrayAttrib = (PFNGLDISABLEVERTEXARRAYATTRIB_PROC*)get_proc("glDisableVertexArrayAttrib");
    glpfEnableVertexArrayAttrib = (PFNGLENABLEVERTEXARRAYATTRIB_PROC*)get_proc("glEnableVertexArrayAttrib");
    glpfFlushMappedNamedBufferRange = (PFNGLFLUSHMAPPEDNAMEDBUFFERRANGE_PROC*)get_proc("glFlushMappedNamedBufferRange");
    glpfGenerateTextureMipmap = (PFNGLGENERATETEXTUREMIPMAP_PROC*)get_proc("glGenerateTextureMipmap");
    glpfGetCompressedTextureImage = (PFNGLGETCOMPRESSEDTEXTUREIMAGE_PROC*)get_proc("glGetCompressedTextureImage");
    glpfGetNamedBufferParameteri64v = (PFNGLGETNAMEDBUFFERPARAMETERI64V_PROC*)get_proc("glGetNamedBufferParameteri64v");
    glpfGetNamedBufferParameteriv = (PFNGLGETNAMEDBUFFERPARAMETERIV_PROC*)get_proc("glGetNamedBufferParameteriv");
    glpfGetNamedBufferPointerv = (PFNGLGETNAMEDBUFFERPOINTERV_PROC*)get_proc("glGetNamedBufferPointerv");
    glpfGetNamedBufferSubData = (PFNGLGETNAMEDBUFFERSUBDATA_PROC*)get_proc("glGetNamedBufferSubData");
    glpfGetNamedFramebufferAttachmentParameteriv = (PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIV_PROC*)get_proc("glGetNamedFramebufferAttachmentParameteriv");
    glpfGetNamedFramebufferParameteriv = (PFNGLGETNAMEDFRAMEBUFFERPARAMETERIV_PROC*)get_proc("glGetNamedFramebufferParameteriv");
    glpfGetNamedRenderbufferParameteriv = (PFNGLGETNAMEDRENDERBUFFERPARAMETERIV_PROC*)get_proc("glGetNamedRenderbufferParameteriv");
    glpfGetQueryBufferObjecti64v = (PFNGLGETQUERYBUFFEROBJECTI64V_PROC*)get_proc("glGetQueryBufferObjecti64v");
    glpfGetQueryBufferObjectiv = (PFNGLGETQUERYBUFFEROBJECTIV_PROC*)get_proc("glGetQueryBufferObjectiv");
    glpfGetQueryBufferObjectui64v = (PFNGLGETQUERYBUFFEROBJECTUI64V_PROC*)get_proc("glGetQueryBufferObjectui64v");
    glpfGetQueryBufferObjectuiv = (PFNGLGETQUERYBUFFEROBJECTUIV_PROC*)get_proc("glGetQueryBufferObjectuiv");
    glpfGetTextureImage = (PFNGLGETTEXTUREIMAGE_PROC*)get_proc("glGetTextureImage");
    glpfGetTextureLevelParameterfv = (PFNGLGETTEXTURELEVELPARAMETERFV_PROC*)get_proc("glGetTextureLevelParameterfv");
    glpfGetTextureLevelParameteriv = (PFNGLGETTEXTURELEVELPARAMETERIV_PROC*)get_proc("glGetTextureLevelParameteriv");
    glpfGetTextureParameterIiv = (PFNGLGETTEXTUREPARAMETERIIV_PROC*)get_proc("glGetTextureParameterIiv");
    glpfGetTextureParameterIuiv = (PFNGLGETTEXTUREPARAMETERIUIV_PROC*)get_proc("glGetTextureParameterIuiv");
    glpfGetTextureParameterfv = (PFNGLGETTEXTUREPARAMETERFV_PROC*)get_proc("glGetTextureParameterfv");
    glpfGetTextureParameteriv = (PFNGLGETTEXTUREPARAMETERIV_PROC*)get_proc("glGetTextureParameteriv");
    glpfGetTransformFeedbacki64_v = (PFNGLGETTRANSFORMFEEDBACKI64_V_PROC*)get_proc("glGetTransformFeedbacki64_v");
    glpfGetTransformFeedbacki_v = (PFNGLGETTRANSFORMFEEDBACKI_V_PROC*)get_proc("glGetTransformFeedbacki_v");
    glpfGetTransformFeedbackiv = (PFNGLGETTRANSFORMFEEDBACKIV_PROC*)get_proc("glGetTransformFeedbackiv");
    glpfGetVertexArrayIndexed64iv = (PFNGLGETVERTEXARRAYINDEXED64IV_PROC*)get_proc("glGetVertexArrayIndexed64iv");
    glpfGetVertexArrayIndexediv = (PFNGLGETVERTEXARRAYINDEXEDIV_PROC*)get_proc("glGetVertexArrayIndexediv");
    glpfGetVertexArrayiv = (PFNGLGETVERTEXARRAYIV_PROC*)get_proc("glGetVertexArrayiv");
    glpfInvalidateNamedFramebufferData = (PFNGLINVALIDATENAMEDFRAMEBUFFERDATA_PROC*)get_proc("glInvalidateNamedFramebufferData");
    glpfInvalidateNamedFramebufferSubData = (PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATA_PROC*)get_proc("glInvalidateNamedFramebufferSubData");
    glpfMapNamedBuffer = (PFNGLMAPNAMEDBUFFER_PROC*)get_proc("glMapNamedBuffer");
    glpfMapNamedBufferRange = (PFNGLMAPNAMEDBUFFERRANGE_PROC*)get_proc("glMapNamedBufferRange");
    glpfNamedBufferData = (PFNGLNAMEDBUFFERDATA_PROC*)get_proc("glNamedBufferData");
    glpfNamedBufferStorage = (PFNGLNAMEDBUFFERSTORAGE_PROC*)get_proc("glNamedBufferStorage");
    glpfNamedBufferSubData = (PFNGLNAMEDBUFFERSUBDATA_PROC*)get_proc("glNamedBufferSubData");
    glpfNamedFramebufferDrawBuffer = (PFNGLNAMEDFRAMEBUFFERDRAWBUFFER_PROC*)get_proc("glNamedFramebufferDrawBuffer");
    glpfNamedFramebufferDrawBuffers = (PFNGLNAMEDFRAMEBUFFERDRAWBUFFERS_PROC*)get_proc("glNamedFramebufferDrawBuffers");
    glpfNamedFramebufferParameteri = (PFNGLNAMEDFRAMEBUFFERPARAMETERI_PROC*)get_proc("glNamedFramebufferParameteri");
    glpfNamedFramebufferReadBuffer = (PFNGLNAMEDFRAMEBUFFERREADBUFFER_PROC*)get_proc("glNamedFramebufferReadBuffer");
    glpfNamedFramebufferRenderbuffer = (PFNGLNAMEDFRAMEBUFFERRENDERBUFFER_PROC*)get_proc("glNamedFramebufferRenderbuffer");
    glpfNamedFramebufferTexture = (PFNGLNAMEDFRAMEBUFFERTEXTURE_PROC*)get_proc("glNamedFramebufferTexture");
    glpfNamedFramebufferTextureLayer = (PFNGLNAMEDFRAMEBUFFERTEXTURELAYER_PROC*)get_proc("glNamedFramebufferTextureLayer");
    glpfNamedRenderbufferStorage = (PFNGLNAMEDRENDERBUFFERSTORAGE_PROC*)get_proc("glNamedRenderbufferStorage");
    glpfNamedRenderbufferStorageMultisample = (PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLE_PROC*)get_proc("glNamedRenderbufferStorageMultisample");
    glpfTextureBuffer = (PFNGLTEXTUREBUFFER_PROC*)get_proc("glTextureBuffer");
    glpfTextureBufferRange = (PFNGLTEXTUREBUFFERRANGE_PROC*)get_proc("glTextureBufferRange");
    glpfTextureParameterIiv = (PFNGLTEXTUREPARAMETERIIV_PROC*)get_proc("glTextureParameterIiv");
    glpfTextureParameterIuiv = (PFNGLTEXTUREPARAMETERIUIV_PROC*)get_proc("glTextureParameterIuiv");
    glpfTextureParameterf = (PFNGLTEXTUREPARAMETERF_PROC*)get_proc("glTextureParameterf");
    glpfTextureParameterfv = (PFNGLTEXTUREPARAMETERFV_PROC*)get_proc("glTextureParameterfv");
    glpfTextureParameteri = (PFNGLTEXTUREPARAMETERI_PROC*)get_proc("glTextureParameteri");
    glpfTextureParameteriv = (PFNGLTEXTUREPARAMETERIV_PROC*)get_proc("glTextureParameteriv");
    glpfTextureStorage1D = (PFNGLTEXTURESTORAGE1D_PROC*)get_proc("glTextureStorage1D");
    glpfTextureStorage2D = (PFNGLTEXTURESTORAGE2D_PROC*)get_proc("glTextureStorage2D");
    glpfTextureStorage2DMultisample = (PFNGLTEXTURESTORAGE2DMULTISAMPLE_PROC*)get_proc("glTextureStorage2DMultisample");
    glpfTextureStorage3D = (PFNGLTEXTURESTORAGE3D_PROC*)get_proc("glTextureStorage3D");
    glpfTextureStorage3DMultisample = (PFNGLTEXTURESTORAGE3DMULTISAMPLE_PROC*)get_proc("glTextureStorage3DMultisample");
    glpfTextureSubImage1D = (PFNGLTEXTURESUBIMAGE1D_PROC*)get_proc("glTextureSubImage1D");
    glpfTextureSubImage2D = (PFNGLTEXTURESUBIMAGE2D_PROC*)get_proc("glTextureSubImage2D");
    glpfTextureSubImage3D = (PFNGLTEXTURESUBIMAGE3D_PROC*)get_proc("glTextureSubImage3D");
    glpfTransformFeedbackBufferBase = (PFNGLTRANSFORMFEEDBACKBUFFERBASE_PROC*)get_proc("glTransformFeedbackBufferBase");
    glpfTransformFeedbackBufferRange = (PFNGLTRANSFORMFEEDBACKBUFFERRANGE_PROC*)get_proc("glTransformFeedbackBufferRange");
    glpfUnmapNamedBuffer = (PFNGLUNMAPNAMEDBUFFER_PROC*)get_proc("glUnmapNamedBuffer");
    glpfVertexArrayAttribBinding = (PFNGLVERTEXARRAYATTRIBBINDING_PROC*)get_proc("glVertexArrayAttribBinding");
    glpfVertexArrayAttribFormat = (PFNGLVERTEXARRAYATTRIBFORMAT_PROC*)get_proc("glVertexArrayAttribFormat");
    glpfVertexArrayAttribIFormat = (PFNGLVERTEXARRAYATTRIBIFORMAT_PROC*)get_proc("glVertexArrayAttribIFormat");
    glpfVertexArrayAttribLFormat = (PFNGLVERTEXARRAYATTRIBLFORMAT_PROC*)get_proc("glVertexArrayAttribLFormat");
    glpfVertexArrayBindingDivisor = (PFNGLVERTEXARRAYBINDINGDIVISOR_PROC*)get_proc("glVertexArrayBindingDivisor");
    glpfVertexArrayElementBuffer = (PFNGLVERTEXARRAYELEMENTBUFFER_PROC*)get_proc("glVertexArrayElementBuffer");
    glpfVertexArrayVertexBuffer = (PFNGLVERTEXARRAYVERTEXBUFFER_PROC*)get_proc("glVertexArrayVertexBuffer");
    glpfVertexArrayVertexBuffers = (PFNGLVERTEXARRAYVERTEXBUFFERS_PROC*)get_proc("glVertexArrayVertexBuffers");


    /* GL_EXT_direct_state_access */

    glpfBindMultiTextureEXT = (PFNGLBINDMULTITEXTUREEXT_PROC*)get_proc("glBindMultiTextureEXT");
    glpfCheckNamedFramebufferStatusEXT = (PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXT_PROC*)get_proc("glCheckNamedFramebufferStatusEXT");
    glpfClearNamedBufferDataEXT = (PFNGLCLEARNAMEDBUFFERDATAEXT_PROC*)get_proc("glClearNamedBufferDataEXT");
    glpfClearNamedBufferSubDataEXT = (PFNGLCLEARNAMEDBUFFERSUBDATAEXT_PROC*)get_proc("glClearNamedBufferSubDataEXT");
    glpfClientAttribDefaultEXT = (PFNGLCLIENTATTRIBDEFAULTEXT_PROC*)get_proc("glClientAttribDefaultEXT");
    glpfCompressedMultiTexImage1DEXT = (PFNGLCOMPRESSEDMULTITEXIMAGE1DEXT_PROC*)get_proc("glCompressedMultiTexImage1DEXT");
    glpfCompressedMultiTexImage2DEXT = (PFNGLCOMPRESSEDMULTITEXIMAGE2DEXT_PROC*)get_proc("glCompressedMultiTexImage2DEXT");
    glpfCompressedMultiTexImage3DEXT = (PFNGLCOMPRESSEDMULTITEXIMAGE3DEXT_PROC*)get_proc("glCompressedMultiTexImage3DEXT");
    glpfCompressedMultiTexSubImage1DEXT = (PFNGLCOMPRESSEDMULTITEXSUBIMAGE1DEXT_PROC*)get_proc("glCompressedMultiTexSubImage1DEXT");
    glpfCompressedMultiTexSubImage2DEXT = (PFNGLCOMPRESSEDMULTITEXSUBIMAGE2DEXT_PROC*)get_proc("glCompressedMultiTexSubImage2DEXT");
    glpfCompressedMultiTexSubImage3DEXT = (PFNGLCOMPRESSEDMULTITEXSUBIMAGE3DEXT_PROC*)get_proc("glCompressedMultiTexSubImage3DEXT");
    glpfCompressedTextureImage1DEXT = (PFNGLCOMPRESSEDTEXTUREIMAGE1DEXT_PROC*)get_proc("glCompressedTextureImage1DEXT");
    glpfCompressedTextureImage2DEXT = (PFNGLCOMPRESSEDTEXTUREIMAGE2DEXT_PROC*)get_proc("glCompressedTextureImage2DEXT");
    glpfCompressedTextureImage3DEXT = (PFNGLCOMPRESSEDTEXTUREIMAGE3DEXT_PROC*)get_proc("glCompressedTextureImage3DEXT");
    glpfCompressedTextureSubImage1DEXT = (PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXT_PROC*)get_proc("glCompressedTextureSubImage1DEXT");
    glpfCompressedTextureSubImage2DEXT = (PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXT_PROC*)get_proc("glCompressedTextureSubImage2DEXT");
    glpfCompressedTextureSubImage3DEXT = (PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXT_PROC*)get_proc("glCompressedTextureSubImage3DEXT");
    glpfCopyMultiTexImage1DEXT = (PFNGLCOPYMULTITEXIMAGE1DEXT_PROC*)get_proc("glCopyMultiTexImage1DEXT");
    glpfCopyMultiTexImage2DEXT = (PFNGLCOPYMULTITEXIMAGE2DEXT_PROC*)get_proc("glCopyMultiTexImage2DEXT");
    glpfCopyMultiTexSubImage1DEXT = (PFNGLCOPYMULTITEXSUBIMAGE1DEXT_PROC*)get_proc("glCopyMultiTexSubImage1DEXT");
    glpfCopyMultiTexSubImage2DEXT = (PFNGLCOPYMULTITEXSUBIMAGE2DEXT_PROC*)get_proc("glCopyMultiTexSubImage2DEXT");
    glpfCopyMultiTexSubImage3DEXT = (PFNGLCOPYMULTITEXSUBIMAGE3DEXT_PROC*)get_proc("glCopyMultiTexSubImage3DEXT");
    glpfCopyTextureImage1DEXT = (PFNGLCOPYTEXTUREIMAGE1DEXT_PROC*)get_proc("glCopyTextureImage1DEXT");
    glpfCopyTextureImage2DEXT = (PFNGLCOPYTEXTUREIMAGE2DEXT_PROC*)get_proc("glCopyTextureImage2DEXT");
    glpfCopyTextureSubImage1DEXT = (PFNGLCOPYTEXTURESUBIMAGE1DEXT_PROC*)get_proc("glCopyTextureSubImage1DEXT");
    glpfCopyTextureSubImage2DEXT = (PFNGLCOPYTEXTURESUBIMAGE2DEXT_PROC*)get_proc("glCopyTextureSubImage2DEXT");
    glpfCopyTextureSubImage3DEXT = (PFNGLCOPYTEXTURESUBIMAGE3DEXT_PROC*)get_proc("glCopyTextureSubImage3DEXT");
    glpfDisableClientStateIndexedEXT = (PFNGLDISABLECLIENTSTATEINDEXEDEXT_PROC*)get_proc("glDisableClientStateIndexedEXT");
    glpfDisableClientStateiEXT = (PFNGLDISABLECLIENTSTATEIEXT_PROC*)get_proc("glDisableClientStateiEXT");
    glpfDisableIndexedEXT = (PFNGLDISABLEINDEXEDEXT_PROC*)get_proc("glDisableIndexedEXT");
    glpfDisableVertexArrayAttribEXT = (PFNGLDISABLEVERTEXARRAYATTRIBEXT_PROC*)get_proc("glDisableVertexArrayAttribEXT");
    glpfDisableVertexArrayEXT = (PFNGLDISABLEVERTEXARRAYEXT_PROC*)get_proc("glDisableVertexArrayEXT");
    glpfEnableClientStateIndexedEXT = (PFNGLENABLECLIENTSTATEINDEXEDEXT_PROC*)get_proc("glEnableClientStateIndexedEXT");
    glpfEnableClientStateiEXT = (PFNGLENABLECLIENTSTATEIEXT_PROC*)get_proc("glEnableClientStateiEXT");
    glpfEnableIndexedEXT = (PFNGLENABLEINDEXEDEXT_PROC*)get_proc("glEnableIndexedEXT");
    glpfEnableVertexArrayAttribEXT = (PFNGLENABLEVERTEXARRAYATTRIBEXT_PROC*)get_proc("glEnableVertexArrayAttribEXT");
    glpfEnableVertexArrayEXT = (PFNGLENABLEVERTEXARRAYEXT_PROC*)get_proc("glEnableVertexArrayEXT");
    glpfFlushMappedNamedBufferRangeEXT = (PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEEXT_PROC*)get_proc("glFlushMappedNamedBufferRangeEXT");
    glpfFramebufferDrawBufferEXT = (PFNGLFRAMEBUFFERDRAWBUFFEREXT_PROC*)get_proc("glFramebufferDrawBufferEXT");
    glpfFramebufferDrawBuffersEXT = (PFNGLFRAMEBUFFERDRAWBUFFERSEXT_PROC*)get_proc("glFramebufferDrawBuffersEXT");
    glpfFramebufferReadBufferEXT = (PFNGLFRAMEBUFFERREADBUFFEREXT_PROC*)get_proc("glFramebufferReadBufferEXT");
    glpfGenerateMultiTexMipmapEXT = (PFNGLGENERATEMULTITEXMIPMAPEXT_PROC*)get_proc("glGenerateMultiTexMipmapEXT");
    glpfGenerateTextureMipmapEXT = (PFNGLGENERATETEXTUREMIPMAPEXT_PROC*)get_proc("glGenerateTextureMipmapEXT");
    glpfGetBooleanIndexedvEXT = (PFNGLGETBOOLEANINDEXEDVEXT_PROC*)get_proc("glGetBooleanIndexedvEXT");
    glpfGetCompressedMultiTexImageEXT = (PFNGLGETCOMPRESSEDMULTITEXIMAGEEXT_PROC*)get_proc("glGetCompressedMultiTexImageEXT");
    glpfGetCompressedTextureImageEXT = (PFNGLGETCOMPRESSEDTEXTUREIMAGEEXT_PROC*)get_proc("glGetCompressedTextureImageEXT");
    glpfGetDoubleIndexedvEXT = (PFNGLGETDOUBLEINDEXEDVEXT_PROC*)get_proc("glGetDoubleIndexedvEXT");
    glpfGetDoublei_vEXT = (PFNGLGETDOUBLEI_VEXT_PROC*)get_proc("glGetDoublei_vEXT");
    glpfGetFloatIndexedvEXT = (PFNGLGETFLOATINDEXEDVEXT_PROC*)get_proc("glGetFloatIndexedvEXT");
    glpfGetFloati_vEXT = (PFNGLGETFLOATI_VEXT_PROC*)get_proc("glGetFloati_vEXT");
    glpfGetFramebufferParameterivEXT = (PFNGLGETFRAMEBUFFERPARAMETERIVEXT_PROC*)get_proc("glGetFramebufferParameterivEXT");
    glpfGetIntegerIndexedvEXT = (PFNGLGETINTEGERINDEXEDVEXT_PROC*)get_proc("glGetIntegerIndexedvEXT");
    glpfGetMultiTexEnvfvEXT = (PFNGLGETMULTITEXENVFVEXT_PROC*)get_proc("glGetMultiTexEnvfvEXT");
    glpfGetMultiTexEnvivEXT = (PFNGLGETMULTITEXENVIVEXT_PROC*)get_proc("glGetMultiTexEnvivEXT");
    glpfGetMultiTexGendvEXT = (PFNGLGETMULTITEXGENDVEXT_PROC*)get_proc("glGetMultiTexGendvEXT");
    glpfGetMultiTexGenfvEXT = (PFNGLGETMULTITEXGENFVEXT_PROC*)get_proc("glGetMultiTexGenfvEXT");
    glpfGetMultiTexGenivEXT = (PFNGLGETMULTITEXGENIVEXT_PROC*)get_proc("glGetMultiTexGenivEXT");
    glpfGetMultiTexImageEXT = (PFNGLGETMULTITEXIMAGEEXT_PROC*)get_proc("glGetMultiTexImageEXT");
    glpfGetMultiTexLevelParameterfvEXT = (PFNGLGETMULTITEXLEVELPARAMETERFVEXT_PROC*)get_proc("glGetMultiTexLevelParameterfvEXT");
    glpfGetMultiTexLevelParameterivEXT = (PFNGLGETMULTITEXLEVELPARAMETERIVEXT_PROC*)get_proc("glGetMultiTexLevelParameterivEXT");
    glpfGetMultiTexParameterIivEXT = (PFNGLGETMULTITEXPARAMETERIIVEXT_PROC*)get_proc("glGetMultiTexParameterIivEXT");
    glpfGetMultiTexParameterIuivEXT = (PFNGLGETMULTITEXPARAMETERIUIVEXT_PROC*)get_proc("glGetMultiTexParameterIuivEXT");
    glpfGetMultiTexParameterfvEXT = (PFNGLGETMULTITEXPARAMETERFVEXT_PROC*)get_proc("glGetMultiTexParameterfvEXT");
    glpfGetMultiTexParameterivEXT = (PFNGLGETMULTITEXPARAMETERIVEXT_PROC*)get_proc("glGetMultiTexParameterivEXT");
    glpfGetNamedBufferParameterivEXT = (PFNGLGETNAMEDBUFFERPARAMETERIVEXT_PROC*)get_proc("glGetNamedBufferParameterivEXT");
    glpfGetNamedBufferPointervEXT = (PFNGLGETNAMEDBUFFERPOINTERVEXT_PROC*)get_proc("glGetNamedBufferPointervEXT");
    glpfGetNamedBufferSubDataEXT = (PFNGLGETNAMEDBUFFERSUBDATAEXT_PROC*)get_proc("glGetNamedBufferSubDataEXT");
    glpfGetNamedFramebufferAttachmentParameterivEXT = (PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXT_PROC*)get_proc("glGetNamedFramebufferAttachmentParameterivEXT");
    glpfGetNamedFramebufferParameterivEXT = (PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVEXT_PROC*)get_proc("glGetNamedFramebufferParameterivEXT");
    glpfGetNamedProgramLocalParameterIivEXT = (PFNGLGETNAMEDPROGRAMLOCALPARAMETERIIVEXT_PROC*)get_proc("glGetNamedProgramLocalParameterIivEXT");
    glpfGetNamedProgramLocalParameterIuivEXT = (PFNGLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXT_PROC*)get_proc("glGetNamedProgramLocalParameterIuivEXT");
    glpfGetNamedProgramLocalParameterdvEXT = (PFNGLGETNAMEDPROGRAMLOCALPARAMETERDVEXT_PROC*)get_proc("glGetNamedProgramLocalParameterdvEXT");
    glpfGetNamedProgramLocalParameterfvEXT = (PFNGLGETNAMEDPROGRAMLOCALPARAMETERFVEXT_PROC*)get_proc("glGetNamedProgramLocalParameterfvEXT");
    glpfGetNamedProgramStringEXT = (PFNGLGETNAMEDPROGRAMSTRINGEXT_PROC*)get_proc("glGetNamedProgramStringEXT");
    glpfGetNamedProgramivEXT = (PFNGLGETNAMEDPROGRAMIVEXT_PROC*)get_proc("glGetNamedProgramivEXT");
    glpfGetNamedRenderbufferParameterivEXT = (PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXT_PROC*)get_proc("glGetNamedRenderbufferParameterivEXT");
    glpfGetPointerIndexedvEXT = (PFNGLGETPOINTERINDEXEDVEXT_PROC*)get_proc("glGetPointerIndexedvEXT");
    glpfGetPointeri_vEXT = (PFNGLGETPOINTERI_VEXT_PROC*)get_proc("glGetPointeri_vEXT");
    glpfGetTextureImageEXT = (PFNGLGETTEXTUREIMAGEEXT_PROC*)get_proc("glGetTextureImageEXT");
    glpfGetTextureLevelParameterfvEXT = (PFNGLGETTEXTURELEVELPARAMETERFVEXT_PROC*)get_proc("glGetTextureLevelParameterfvEXT");
    glpfGetTextureLevelParameterivEXT = (PFNGLGETTEXTURELEVELPARAMETERIVEXT_PROC*)get_proc("glGetTextureLevelParameterivEXT");
    glpfGetTextureParameterIivEXT = (PFNGLGETTEXTUREPARAMETERIIVEXT_PROC*)get_proc("glGetTextureParameterIivEXT");
    glpfGetTextureParameterIuivEXT = (PFNGLGETTEXTUREPARAMETERIUIVEXT_PROC*)get_proc("glGetTextureParameterIuivEXT");
    glpfGetTextureParameterfvEXT = (PFNGLGETTEXTUREPARAMETERFVEXT_PROC*)get_proc("glGetTextureParameterfvEXT");
    glpfGetTextureParameterivEXT = (PFNGLGETTEXTUREPARAMETERIVEXT_PROC*)get_proc("glGetTextureParameterivEXT");
    glpfGetVertexArrayIntegeri_vEXT = (PFNGLGETVERTEXARRAYINTEGERI_VEXT_PROC*)get_proc("glGetVertexArrayIntegeri_vEXT");
    glpfGetVertexArrayIntegervEXT = (PFNGLGETVERTEXARRAYINTEGERVEXT_PROC*)get_proc("glGetVertexArrayIntegervEXT");
    glpfGetVertexArrayPointeri_vEXT = (PFNGLGETVERTEXARRAYPOINTERI_VEXT_PROC*)get_proc("glGetVertexArrayPointeri_vEXT");
    glpfGetVertexArrayPointervEXT = (PFNGLGETVERTEXARRAYPOINTERVEXT_PROC*)get_proc("glGetVertexArrayPointervEXT");
    glpfIsEnabledIndexedEXT = (PFNGLISENABLEDINDEXEDEXT_PROC*)get_proc("glIsEnabledIndexedEXT");
    glpfMapNamedBufferEXT = (PFNGLMAPNAMEDBUFFEREXT_PROC*)get_proc("glMapNamedBufferEXT");
    glpfMapNamedBufferRangeEXT = (PFNGLMAPNAMEDBUFFERRANGEEXT_PROC*)get_proc("glMapNamedBufferRangeEXT");
    glpfMatrixFrustumEXT = (PFNGLMATRIXFRUSTUMEXT_PROC*)get_proc("glMatrixFrustumEXT");
    glpfMatrixLoadIdentityEXT = (PFNGLMATRIXLOADIDENTITYEXT_PROC*)get_proc("glMatrixLoadIdentityEXT");
    glpfMatrixLoadTransposedEXT = (PFNGLMATRIXLOADTRANSPOSEDEXT_PROC*)get_proc("glMatrixLoadTransposedEXT");
    glpfMatrixLoadTransposefEXT = (PFNGLMATRIXLOADTRANSPOSEFEXT_PROC*)get_proc("glMatrixLoadTransposefEXT");
    glpfMatrixLoaddEXT = (PFNGLMATRIXLOADDEXT_PROC*)get_proc("glMatrixLoaddEXT");
    glpfMatrixLoadfEXT = (PFNGLMATRIXLOADFEXT_PROC*)get_proc("glMatrixLoadfEXT");
    glpfMatrixMultTransposedEXT = (PFNGLMATRIXMULTTRANSPOSEDEXT_PROC*)get_proc("glMatrixMultTransposedEXT");
    glpfMatrixMultTransposefEXT = (PFNGLMATRIXMULTTRANSPOSEFEXT_PROC*)get_proc("glMatrixMultTransposefEXT");
    glpfMatrixMultdEXT = (PFNGLMATRIXMULTDEXT_PROC*)get_proc("glMatrixMultdEXT");
    glpfMatrixMultfEXT = (PFNGLMATRIXMULTFEXT_PROC*)get_proc("glMatrixMultfEXT");
    glpfMatrixOrthoEXT = (PFNGLMATRIXORTHOEXT_PROC*)get_proc("glMatrixOrthoEXT");
    glpfMatrixPopEXT = (PFNGLMATRIXPOPEXT_PROC*)get_proc("glMatrixPopEXT");
    glpfMatrixPushEXT = (PFNGLMATRIXPUSHEXT_PROC*)get_proc("glMatrixPushEXT");
    glpfMatrixRotatedEXT = (PFNGLMATRIXROTATEDEXT_PROC*)get_proc("glMatrixRotatedEXT");
    glpfMatrixRotatefEXT = (PFNGLMATRIXROTATEFEXT_PROC*)get_proc("glMatrixRotatefEXT");
    glpfMatrixScaledEXT = (PFNGLMATRIXSCALEDEXT_PROC*)get_proc("glMatrixScaledEXT");
    glpfMatrixScalefEXT = (PFNGLMATRIXSCALEFEXT_PROC*)get_proc("glMatrixScalefEXT");
    glpfMatrixTranslatedEXT = (PFNGLMATRIXTRANSLATEDEXT_PROC*)get_proc("glMatrixTranslatedEXT");
    glpfMatrixTranslatefEXT = (PFNGLMATRIXTRANSLATEFEXT_PROC*)get_proc("glMatrixTranslatefEXT");
    glpfMultiTexBufferEXT = (PFNGLMULTITEXBUFFEREXT_PROC*)get_proc("glMultiTexBufferEXT");
    glpfMultiTexCoordPointerEXT = (PFNGLMULTITEXCOORDPOINTEREXT_PROC*)get_proc("glMultiTexCoordPointerEXT");
    glpfMultiTexEnvfEXT = (PFNGLMULTITEXENVFEXT_PROC*)get_proc("glMultiTexEnvfEXT");
    glpfMultiTexEnvfvEXT = (PFNGLMULTITEXENVFVEXT_PROC*)get_proc("glMultiTexEnvfvEXT");
    glpfMultiTexEnviEXT = (PFNGLMULTITEXENVIEXT_PROC*)get_proc("glMultiTexEnviEXT");
    glpfMultiTexEnvivEXT = (PFNGLMULTITEXENVIVEXT_PROC*)get_proc("glMultiTexEnvivEXT");
    glpfMultiTexGendEXT = (PFNGLMULTITEXGENDEXT_PROC*)get_proc("glMultiTexGendEXT");
    glpfMultiTexGendvEXT = (PFNGLMULTITEXGENDVEXT_PROC*)get_proc("glMultiTexGendvEXT");
    glpfMultiTexGenfEXT = (PFNGLMULTITEXGENFEXT_PROC*)get_proc("glMultiTexGenfEXT");
    glpfMultiTexGenfvEXT = (PFNGLMULTITEXGENFVEXT_PROC*)get_proc("glMultiTexGenfvEXT");
    glpfMultiTexGeniEXT = (PFNGLMULTITEXGENIEXT_PROC*)get_proc("glMultiTexGeniEXT");
    glpfMultiTexGenivEXT = (PFNGLMULTITEXGENIVEXT_PROC*)get_proc("glMultiTexGenivEXT");
    glpfMultiTexImage1DEXT = (PFNGLMULTITEXIMAGE1DEXT_PROC*)get_proc("glMultiTexImage1DEXT");
    glpfMultiTexImage2DEXT = (PFNGLMULTITEXIMAGE2DEXT_PROC*)get_proc("glMultiTexImage2DEXT");
    glpfMultiTexImage3DEXT = (PFNGLMULTITEXIMAGE3DEXT_PROC*)get_proc("glMultiTexImage3DEXT");
    glpfMultiTexParameterIivEXT = (PFNGLMULTITEXPARAMETERIIVEXT_PROC*)get_proc("glMultiTexParameterIivEXT");
    glpfMultiTexParameterIuivEXT = (PFNGLMULTITEXPARAMETERIUIVEXT_PROC*)get_proc("glMultiTexParameterIuivEXT");
    glpfMultiTexParameterfEXT = (PFNGLMULTITEXPARAMETERFEXT_PROC*)get_proc("glMultiTexParameterfEXT");
    glpfMultiTexParameterfvEXT = (PFNGLMULTITEXPARAMETERFVEXT_PROC*)get_proc("glMultiTexParameterfvEXT");
    glpfMultiTexParameteriEXT = (PFNGLMULTITEXPARAMETERIEXT_PROC*)get_proc("glMultiTexParameteriEXT");
    glpfMultiTexParameterivEXT = (PFNGLMULTITEXPARAMETERIVEXT_PROC*)get_proc("glMultiTexParameterivEXT");
    glpfMultiTexRenderbufferEXT = (PFNGLMULTITEXRENDERBUFFEREXT_PROC*)get_proc("glMultiTexRenderbufferEXT");
    glpfMultiTexSubImage1DEXT = (PFNGLMULTITEXSUBIMAGE1DEXT_PROC*)get_proc("glMultiTexSubImage1DEXT");
    glpfMultiTexSubImage2DEXT = (PFNGLMULTITEXSUBIMAGE2DEXT_PROC*)get_proc("glMultiTexSubImage2DEXT");
    glpfMultiTexSubImage3DEXT = (PFNGLMULTITEXSUBIMAGE3DEXT_PROC*)get_proc("glMultiTexSubImage3DEXT");
    glpfNamedBufferDataEXT = (PFNGLNAMEDBUFFERDATAEXT_PROC*)get_proc("glNamedBufferDataEXT");
    glpfNamedBufferStorageEXT = (PFNGLNAMEDBUFFERSTORAGEEXT_PROC*)get_proc("glNamedBufferStorageEXT");
    glpfNamedBufferSubDataEXT = (PFNGLNAMEDBUFFERSUBDATAEXT_PROC*)get_proc("glNamedBufferSubDataEXT");
    glpfNamedCopyBufferSubDataEXT = (PFNGLNAMEDCOPYBUFFERSUBDATAEXT_PROC*)get_proc("glNamedCopyBufferSubDataEXT");
    glpfNamedFramebufferParameteriEXT = (PFNGLNAMEDFRAMEBUFFERPARAMETERIEXT_PROC*)get_proc("glNamedFramebufferParameteriEXT");
    glpfNamedFramebufferRenderbufferEXT = (PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXT_PROC*)get_proc("glNamedFramebufferRenderbufferEXT");
    glpfNamedFramebufferTexture1DEXT = (PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXT_PROC*)get_proc("glNamedFramebufferTexture1DEXT");
    glpfNamedFramebufferTexture2DEXT = (PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXT_PROC*)get_proc("glNamedFramebufferTexture2DEXT");
    glpfNamedFramebufferTexture3DEXT = (PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXT_PROC*)get_proc("glNamedFramebufferTexture3DEXT");
    glpfNamedFramebufferTextureEXT = (PFNGLNAMEDFRAMEBUFFERTEXTUREEXT_PROC*)get_proc("glNamedFramebufferTextureEXT");
    glpfNamedFramebufferTextureFaceEXT = (PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXT_PROC*)get_proc("glNamedFramebufferTextureFaceEXT");
    glpfNamedFramebufferTextureLayerEXT = (PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXT_PROC*)get_proc("glNamedFramebufferTextureLayerEXT");
    glpfNamedProgramLocalParameter4dEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETER4DEXT_PROC*)get_proc("glNamedProgramLocalParameter4dEXT");
    glpfNamedProgramLocalParameter4dvEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETER4DVEXT_PROC*)get_proc("glNamedProgramLocalParameter4dvEXT");
    glpfNamedProgramLocalParameter4fEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETER4FEXT_PROC*)get_proc("glNamedProgramLocalParameter4fEXT");
    glpfNamedProgramLocalParameter4fvEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETER4FVEXT_PROC*)get_proc("glNamedProgramLocalParameter4fvEXT");
    glpfNamedProgramLocalParameterI4iEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERI4IEXT_PROC*)get_proc("glNamedProgramLocalParameterI4iEXT");
    glpfNamedProgramLocalParameterI4ivEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERI4IVEXT_PROC*)get_proc("glNamedProgramLocalParameterI4ivEXT");
    glpfNamedProgramLocalParameterI4uiEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIEXT_PROC*)get_proc("glNamedProgramLocalParameterI4uiEXT");
    glpfNamedProgramLocalParameterI4uivEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIVEXT_PROC*)get_proc("glNamedProgramLocalParameterI4uivEXT");
    glpfNamedProgramLocalParameters4fvEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERS4FVEXT_PROC*)get_proc("glNamedProgramLocalParameters4fvEXT");
    glpfNamedProgramLocalParametersI4ivEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERSI4IVEXT_PROC*)get_proc("glNamedProgramLocalParametersI4ivEXT");
    glpfNamedProgramLocalParametersI4uivEXT = (PFNGLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXT_PROC*)get_proc("glNamedProgramLocalParametersI4uivEXT");
    glpfNamedProgramStringEXT = (PFNGLNAMEDPROGRAMSTRINGEXT_PROC*)get_proc("glNamedProgramStringEXT");
    glpfNamedRenderbufferStorageEXT = (PFNGLNAMEDRENDERBUFFERSTORAGEEXT_PROC*)get_proc("glNamedRenderbufferStorageEXT");
    glpfNamedRenderbufferStorageMultisampleCoverageEXT = (PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXT_PROC*)get_proc("glNamedRenderbufferStorageMultisampleCoverageEXT");
    glpfNamedRenderbufferStorageMultisampleEXT = (PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXT_PROC*)get_proc("glNamedRenderbufferStorageMultisampleEXT");
    glpfProgramUniform1dEXT = (PFNGLPROGRAMUNIFORM1DEXT_PROC*)get_proc("glProgramUniform1dEXT");
    glpfProgramUniform1dvEXT = (PFNGLPROGRAMUNIFORM1DVEXT_PROC*)get_proc("glProgramUniform1dvEXT");
    glpfProgramUniform1fEXT = (PFNGLPROGRAMUNIFORM1FEXT_PROC*)get_proc("glProgramUniform1fEXT");
    glpfProgramUniform1fvEXT = (PFNGLPROGRAMUNIFORM1FVEXT_PROC*)get_proc("glProgramUniform1fvEXT");
    glpfProgramUniform1iEXT = (PFNGLPROGRAMUNIFORM1IEXT_PROC*)get_proc("glProgramUniform1iEXT");
    glpfProgramUniform1ivEXT = (PFNGLPROGRAMUNIFORM1IVEXT_PROC*)get_proc("glProgramUniform1ivEXT");
    glpfProgramUniform1uiEXT = (PFNGLPROGRAMUNIFORM1UIEXT_PROC*)get_proc("glProgramUniform1uiEXT");
    glpfProgramUniform1uivEXT = (PFNGLPROGRAMUNIFORM1UIVEXT_PROC*)get_proc("glProgramUniform1uivEXT");
    glpfProgramUniform2dEXT = (PFNGLPROGRAMUNIFORM2DEXT_PROC*)get_proc("glProgramUniform2dEXT");
    glpfProgramUniform2dvEXT = (PFNGLPROGRAMUNIFORM2DVEXT_PROC*)get_proc("glProgramUniform2dvEXT");
    glpfProgramUniform2fEXT = (PFNGLPROGRAMUNIFORM2FEXT_PROC*)get_proc("glProgramUniform2fEXT");
    glpfProgramUniform2fvEXT = (PFNGLPROGRAMUNIFORM2FVEXT_PROC*)get_proc("glProgramUniform2fvEXT");
    glpfProgramUniform2iEXT = (PFNGLPROGRAMUNIFORM2IEXT_PROC*)get_proc("glProgramUniform2iEXT");
    glpfProgramUniform2ivEXT = (PFNGLPROGRAMUNIFORM2IVEXT_PROC*)get_proc("glProgramUniform2ivEXT");
    glpfProgramUniform2uiEXT = (PFNGLPROGRAMUNIFORM2UIEXT_PROC*)get_proc("glProgramUniform2uiEXT");
    glpfProgramUniform2uivEXT = (PFNGLPROGRAMUNIFORM2UIVEXT_PROC*)get_proc("glProgramUniform2uivEXT");
    glpfProgramUniform3dEXT = (PFNGLPROGRAMUNIFORM3DEXT_PROC*)get_proc("glProgramUniform3dEXT");
    glpfProgramUniform3dvEXT = (PFNGLPROGRAMUNIFORM3DVEXT_PROC*)get_proc("glProgramUniform3dvEXT");
    glpfProgramUniform3fEXT = (PFNGLPROGRAMUNIFORM3FEXT_PROC*)get_proc("glProgramUniform3fEXT");
    glpfProgramUniform3fvEXT = (PFNGLPROGRAMUNIFORM3FVEXT_PROC*)get_proc("glProgramUniform3fvEXT");
    glpfProgramUniform3iEXT = (PFNGLPROGRAMUNIFORM3IEXT_PROC*)get_proc("glProgramUniform3iEXT");
    glpfProgramUniform3ivEXT = (PFNGLPROGRAMUNIFORM3IVEXT_PROC*)get_proc("glProgramUniform3ivEXT");
    glpfProgramUniform3uiEXT = (PFNGLPROGRAMUNIFORM3UIEXT_PROC*)get_proc("glProgramUniform3uiEXT");
    glpfProgramUniform3uivEXT = (PFNGLPROGRAMUNIFORM3UIVEXT_PROC*)get_proc("glProgramUniform3uivEXT");
    glpfProgramUniform4dEXT = (PFNGLPROGRAMUNIFORM4DEXT_PROC*)get_proc("glProgramUniform4dEXT");
    glpfProgramUniform4dvEXT = (PFNGLPROGRAMUNIFORM4DVEXT_PROC*)get_proc("glProgramUniform4dvEXT");
    glpfProgramUniform4fEXT = (PFNGLPROGRAMUNIFORM4FEXT_PROC*)get_proc("glProgramUniform4fEXT");
    glpfProgramUniform4fvEXT = (PFNGLPROGRAMUNIFORM4FVEXT_PROC*)get_proc("glProgramUniform4fvEXT");
    glpfProgramUniform4iEXT = (PFNGLPROGRAMUNIFORM4IEXT_PROC*)get_proc("glProgramUniform4iEXT");
    glpfProgramUniform4ivEXT = (PFNGLPROGRAMUNIFORM4IVEXT_PROC*)get_proc("glProgramUniform4ivEXT");
    glpfProgramUniform4uiEXT = (PFNGLPROGRAMUNIFORM4UIEXT_PROC*)get_proc("glProgramUniform4uiEXT");
    glpfProgramUniform4uivEXT = (PFNGLPROGRAMUNIFORM4UIVEXT_PROC*)get_proc("glProgramUniform4uivEXT");
    glpfProgramUniformMatrix2dvEXT = (PFNGLPROGRAMUNIFORMMATRIX2DVEXT_PROC*)get_proc("glProgramUniformMatrix2dvEXT");
    glpfProgramUniformMatrix2fvEXT = (PFNGLPROGRAMUNIFORMMATRIX2FVEXT_PROC*)get_proc("glProgramUniformMatrix2fvEXT");
    glpfProgramUniformMatrix2x3dvEXT = (PFNGLPROGRAMUNIFORMMATRIX2X3DVEXT_PROC*)get_proc("glProgramUniformMatrix2x3dvEXT");
    glpfProgramUniformMatrix2x3fvEXT = (PFNGLPROGRAMUNIFORMMATRIX2X3FVEXT_PROC*)get_proc("glProgramUniformMatrix2x3fvEXT");
    glpfProgramUniformMatrix2x4dvEXT = (PFNGLPROGRAMUNIFORMMATRIX2X4DVEXT_PROC*)get_proc("glProgramUniformMatrix2x4dvEXT");
    glpfProgramUniformMatrix2x4fvEXT = (PFNGLPROGRAMUNIFORMMATRIX2X4FVEXT_PROC*)get_proc("glProgramUniformMatrix2x4fvEXT");
    glpfProgramUniformMatrix3dvEXT = (PFNGLPROGRAMUNIFORMMATRIX3DVEXT_PROC*)get_proc("glProgramUniformMatrix3dvEXT");
    glpfProgramUniformMatrix3fvEXT = (PFNGLPROGRAMUNIFORMMATRIX3FVEXT_PROC*)get_proc("glProgramUniformMatrix3fvEXT");
    glpfProgramUniformMatrix3x2dvEXT = (PFNGLPROGRAMUNIFORMMATRIX3X2DVEXT_PROC*)get_proc("glProgramUniformMatrix3x2dvEXT");
    glpfProgramUniformMatrix3x2fvEXT = (PFNGLPROGRAMUNIFORMMATRIX3X2FVEXT_PROC*)get_proc("glProgramUniformMatrix3x2fvEXT");
    glpfProgramUniformMatrix3x4dvEXT = (PFNGLPROGRAMUNIFORMMATRIX3X4DVEXT_PROC*)get_proc("glProgramUniformMatrix3x4dvEXT");
    glpfProgramUniformMatrix3x4fvEXT = (PFNGLPROGRAMUNIFORMMATRIX3X4FVEXT_PROC*)get_proc("glProgramUniformMatrix3x4fvEXT");
    glpfProgramUniformMatrix4dvEXT = (PFNGLPROGRAMUNIFORMMATRIX4DVEXT_PROC*)get_proc("glProgramUniformMatrix4dvEXT");
    glpfProgramUniformMatrix4fvEXT = (PFNGLPROGRAMUNIFORMMATRIX4FVEXT_PROC*)get_proc("glProgramUniformMatrix4fvEXT");
    glpfProgramUniformMatrix4x2dvEXT = (PFNGLPROGRAMUNIFORMMATRIX4X2DVEXT_PROC*)get_proc("glProgramUniformMatrix4x2dvEXT");
    glpfProgramUniformMatrix4x2fvEXT = (PFNGLPROGRAMUNIFORMMATRIX4X2FVEXT_PROC*)get_proc("glProgramUniformMatrix4x2fvEXT");
    glpfProgramUniformMatrix4x3dvEXT = (PFNGLPROGRAMUNIFORMMATRIX4X3DVEXT_PROC*)get_proc("glProgramUniformMatrix4x3dvEXT");
    glpfProgramUniformMatrix4x3fvEXT = (PFNGLPROGRAMUNIFORMMATRIX4X3FVEXT_PROC*)get_proc("glProgramUniformMatrix4x3fvEXT");
    glpfPushClientAttribDefaultEXT = (PFNGLPUSHCLIENTATTRIBDEFAULTEXT_PROC*)get_proc("glPushClientAttribDefaultEXT");
    glpfTextureBufferEXT = (PFNGLTEXTUREBUFFEREXT_PROC*)get_proc("glTextureBufferEXT");
    glpfTextureBufferRangeEXT = (PFNGLTEXTUREBUFFERRANGEEXT_PROC*)get_proc("glTextureBufferRangeEXT");
    glpfTextureImage1DEXT = (PFNGLTEXTUREIMAGE1DEXT_PROC*)get_proc("glTextureImage1DEXT");
    glpfTextureImage2DEXT = (PFNGLTEXTUREIMAGE2DEXT_PROC*)get_proc("glTextureImage2DEXT");
    glpfTextureImage3DEXT = (PFNGLTEXTUREIMAGE3DEXT_PROC*)get_proc("glTextureImage3DEXT");
    glpfTexturePageCommitmentEXT = (PFNGLTEXTUREPAGECOMMITMENTEXT_PROC*)get_proc("glTexturePageCommitmentEXT");
    glpfTextureParameterIivEXT = (PFNGLTEXTUREPARAMETERIIVEXT_PROC*)get_proc("glTextureParameterIivEXT");
    glpfTextureParameterIuivEXT = (PFNGLTEXTUREPARAMETERIUIVEXT_PROC*)get_proc("glTextureParameterIuivEXT");
    glpfTextureParameterfEXT = (PFNGLTEXTUREPARAMETERFEXT_PROC*)get_proc("glTextureParameterfEXT");
    glpfTextureParameterfvEXT = (PFNGLTEXTUREPARAMETERFVEXT_PROC*)get_proc("glTextureParameterfvEXT");
    glpfTextureParameteriEXT = (PFNGLTEXTUREPARAMETERIEXT_PROC*)get_proc("glTextureParameteriEXT");
    glpfTextureParameterivEXT = (PFNGLTEXTUREPARAMETERIVEXT_PROC*)get_proc("glTextureParameterivEXT");
    glpfTextureRenderbufferEXT = (PFNGLTEXTURERENDERBUFFEREXT_PROC*)get_proc("glTextureRenderbufferEXT");
    glpfTextureStorage1DEXT = (PFNGLTEXTURESTORAGE1DEXT_PROC*)get_proc("glTextureStorage1DEXT");
    glpfTextureStorage2DEXT = (PFNGLTEXTURESTORAGE2DEXT_PROC*)get_proc("glTextureStorage2DEXT");
    glpfTextureStorage2DMultisampleEXT = (PFNGLTEXTURESTORAGE2DMULTISAMPLEEXT_PROC*)get_proc("glTextureStorage2DMultisampleEXT");
    glpfTextureStorage3DEXT = (PFNGLTEXTURESTORAGE3DEXT_PROC*)get_proc("glTextureStorage3DEXT");
    glpfTextureStorage3DMultisampleEXT = (PFNGLTEXTURESTORAGE3DMULTISAMPLEEXT_PROC*)get_proc("glTextureStorage3DMultisampleEXT");
    glpfTextureSubImage1DEXT = (PFNGLTEXTURESUBIMAGE1DEXT_PROC*)get_proc("glTextureSubImage1DEXT");
    glpfTextureSubImage2DEXT = (PFNGLTEXTURESUBIMAGE2DEXT_PROC*)get_proc("glTextureSubImage2DEXT");
    glpfTextureSubImage3DEXT = (PFNGLTEXTURESUBIMAGE3DEXT_PROC*)get_proc("glTextureSubImage3DEXT");
    glpfUnmapNamedBufferEXT = (PFNGLUNMAPNAMEDBUFFEREXT_PROC*)get_proc("glUnmapNamedBufferEXT");
    glpfVertexArrayBindVertexBufferEXT = (PFNGLVERTEXARRAYBINDVERTEXBUFFEREXT_PROC*)get_proc("glVertexArrayBindVertexBufferEXT");
    glpfVertexArrayColorOffsetEXT = (PFNGLVERTEXARRAYCOLOROFFSETEXT_PROC*)get_proc("glVertexArrayColorOffsetEXT");
    glpfVertexArrayEdgeFlagOffsetEXT = (PFNGLVERTEXARRAYEDGEFLAGOFFSETEXT_PROC*)get_proc("glVertexArrayEdgeFlagOffsetEXT");
    glpfVertexArrayFogCoordOffsetEXT = (PFNGLVERTEXARRAYFOGCOORDOFFSETEXT_PROC*)get_proc("glVertexArrayFogCoordOffsetEXT");
    glpfVertexArrayIndexOffsetEXT = (PFNGLVERTEXARRAYINDEXOFFSETEXT_PROC*)get_proc("glVertexArrayIndexOffsetEXT");
    glpfVertexArrayMultiTexCoordOffsetEXT = (PFNGLVERTEXARRAYMULTITEXCOORDOFFSETEXT_PROC*)get_proc("glVertexArrayMultiTexCoordOffsetEXT");
    glpfVertexArrayNormalOffsetEXT = (PFNGLVERTEXARRAYNORMALOFFSETEXT_PROC*)get_proc("glVertexArrayNormalOffsetEXT");
    glpfVertexArraySecondaryColorOffsetEXT = (PFNGLVERTEXARRAYSECONDARYCOLOROFFSETEXT_PROC*)get_proc("glVertexArraySecondaryColorOffsetEXT");
    glpfVertexArrayTexCoordOffsetEXT = (PFNGLVERTEXARRAYTEXCOORDOFFSETEXT_PROC*)get_proc("glVertexArrayTexCoordOffsetEXT");
    glpfVertexArrayVertexAttribBindingEXT = (PFNGLVERTEXARRAYVERTEXATTRIBBINDINGEXT_PROC*)get_proc("glVertexArrayVertexAttribBindingEXT");
    glpfVertexArrayVertexAttribDivisorEXT = (PFNGLVERTEXARRAYVERTEXATTRIBDIVISOREXT_PROC*)get_proc("glVertexArrayVertexAttribDivisorEXT");
    glpfVertexArrayVertexAttribFormatEXT = (PFNGLVERTEXARRAYVERTEXATTRIBFORMATEXT_PROC*)get_proc("glVertexArrayVertexAttribFormatEXT");
    glpfVertexArrayVertexAttribIFormatEXT = (PFNGLVERTEXARRAYVERTEXATTRIBIFORMATEXT_PROC*)get_proc("glVertexArrayVertexAttribIFormatEXT");
    glpfVertexArrayVertexAttribIOffsetEXT = (PFNGLVERTEXARRAYVERTEXATTRIBIOFFSETEXT_PROC*)get_proc("glVertexArrayVertexAttribIOffsetEXT");
    glpfVertexArrayVertexAttribLFormatEXT = (PFNGLVERTEXARRAYVERTEXATTRIBLFORMATEXT_PROC*)get_proc("glVertexArrayVertexAttribLFormatEXT");
    glpfVertexArrayVertexAttribLOffsetEXT = (PFNGLVERTEXARRAYVERTEXATTRIBLOFFSETEXT_PROC*)get_proc("glVertexArrayVertexAttribLOffsetEXT");
    glpfVertexArrayVertexAttribOffsetEXT = (PFNGLVERTEXARRAYVERTEXATTRIBOFFSETEXT_PROC*)get_proc("glVertexArrayVertexAttribOffsetEXT");
    glpfVertexArrayVertexBindingDivisorEXT = (PFNGLVERTEXARRAYVERTEXBINDINGDIVISOREXT_PROC*)get_proc("glVertexArrayVertexBindingDivisorEXT");
    glpfVertexArrayVertexOffsetEXT = (PFNGLVERTEXARRAYVERTEXOFFSETEXT_PROC*)get_proc("glVertexArrayVertexOffsetEXT");


    /* GL_KHR_texture_compression_astc_hdr */



    /* GL_KHR_texture_compression_astc_ldr */



    /* GL_VERSION_1_2 */

    glpfCopyTexSubImage3D = (PFNGLCOPYTEXSUBIMAGE3D_PROC*)get_proc("glCopyTexSubImage3D");
    glpfDrawRangeElements = (PFNGLDRAWRANGEELEMENTS_PROC*)get_proc("glDrawRangeElements");
    glpfTexImage3D = (PFNGLTEXIMAGE3D_PROC*)get_proc("glTexImage3D");
    glpfTexSubImage3D = (PFNGLTEXSUBIMAGE3D_PROC*)get_proc("glTexSubImage3D");


    /* GL_VERSION_1_3 */

    glpfActiveTexture = (PFNGLACTIVETEXTURE_PROC*)get_proc("glActiveTexture");
    glpfCompressedTexImage1D = (PFNGLCOMPRESSEDTEXIMAGE1D_PROC*)get_proc("glCompressedTexImage1D");
    glpfCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2D_PROC*)get_proc("glCompressedTexImage2D");
    glpfCompressedTexImage3D = (PFNGLCOMPRESSEDTEXIMAGE3D_PROC*)get_proc("glCompressedTexImage3D");
    glpfCompressedTexSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1D_PROC*)get_proc("glCompressedTexSubImage1D");
    glpfCompressedTexSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2D_PROC*)get_proc("glCompressedTexSubImage2D");
    glpfCompressedTexSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3D_PROC*)get_proc("glCompressedTexSubImage3D");
    glpfGetCompressedTexImage = (PFNGLGETCOMPRESSEDTEXIMAGE_PROC*)get_proc("glGetCompressedTexImage");
    glpfSampleCoverage = (PFNGLSAMPLECOVERAGE_PROC*)get_proc("glSampleCoverage");


    /* GL_VERSION_1_4 */

    glpfBlendColor = (PFNGLBLENDCOLOR_PROC*)get_proc("glBlendColor");
    glpfBlendEquation = (PFNGLBLENDEQUATION_PROC*)get_proc("glBlendEquation");
    glpfBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATE_PROC*)get_proc("glBlendFuncSeparate");
    glpfMultiDrawArrays = (PFNGLMULTIDRAWARRAYS_PROC*)get_proc("glMultiDrawArrays");
    glpfMultiDrawElements = (PFNGLMULTIDRAWELEMENTS_PROC*)get_proc("glMultiDrawElements");
    glpfPointParameterf = (PFNGLPOINTPARAMETERF_PROC*)get_proc("glPointParameterf");
    glpfPointParameterfv = (PFNGLPOINTPARAMETERFV_PROC*)get_proc("glPointParameterfv");
    glpfPointParameteri = (PFNGLPOINTPARAMETERI_PROC*)get_proc("glPointParameteri");
    glpfPointParameteriv = (PFNGLPOINTPARAMETERIV_PROC*)get_proc("glPointParameteriv");


    /* GL_VERSION_1_5 */

    glpfBeginQuery = (PFNGLBEGINQUERY_PROC*)get_proc("glBeginQuery");
    glpfBindBuffer = (PFNGLBINDBUFFER_PROC*)get_proc("glBindBuffer");
    glpfBufferData = (PFNGLBUFFERDATA_PROC*)get_proc("glBufferData");
    glpfBufferSubData = (PFNGLBUFFERSUBDATA_PROC*)get_proc("glBufferSubData");
    glpfDeleteBuffers = (PFNGLDELETEBUFFERS_PROC*)get_proc("glDeleteBuffers");
    glpfDeleteQueries = (PFNGLDELETEQUERIES_PROC*)get_proc("glDeleteQueries");
    glpfEndQuery = (PFNGLENDQUERY_PROC*)get_proc("glEndQuery");
    glpfGenBuffers = (PFNGLGENBUFFERS_PROC*)get_proc("glGenBuffers");
    glpfGenQueries = (PFNGLGENQUERIES_PROC*)get_proc("glGenQueries");
    glpfGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIV_PROC*)get_proc("glGetBufferParameteriv");
    glpfGetBufferPointerv = (PFNGLGETBUFFERPOINTERV_PROC*)get_proc("glGetBufferPointerv");
    glpfGetBufferSubData = (PFNGLGETBUFFERSUBDATA_PROC*)get_proc("glGetBufferSubData");
    glpfGetQueryObjectiv = (PFNGLGETQUERYOBJECTIV_PROC*)get_proc("glGetQueryObjectiv");
    glpfGetQueryObjectuiv = (PFNGLGETQUERYOBJECTUIV_PROC*)get_proc("glGetQueryObjectuiv");
    glpfGetQueryiv = (PFNGLGETQUERYIV_PROC*)get_proc("glGetQueryiv");
    glpfIsBuffer = (PFNGLISBUFFER_PROC*)get_proc("glIsBuffer");
    glpfIsQuery = (PFNGLISQUERY_PROC*)get_proc("glIsQuery");
    glpfMapBuffer = (PFNGLMAPBUFFER_PROC*)get_proc("glMapBuffer");
    glpfUnmapBuffer = (PFNGLUNMAPBUFFER_PROC*)get_proc("glUnmapBuffer");


    /* GL_VERSION_2_0 */

    glpfAttachShader = (PFNGLATTACHSHADER_PROC*)get_proc("glAttachShader");
    glpfBindAttribLocation = (PFNGLBINDATTRIBLOCATION_PROC*)get_proc("glBindAttribLocation");
    glpfBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATE_PROC*)get_proc("glBlendEquationSeparate");
    glpfCompileShader = (PFNGLCOMPILESHADER_PROC*)get_proc("glCompileShader");
    glpfCreateProgram = (PFNGLCREATEPROGRAM_PROC*)get_proc("glCreateProgram");
    glpfCreateShader = (PFNGLCREATESHADER_PROC*)get_proc("glCreateShader");
    glpfDeleteProgram = (PFNGLDELETEPROGRAM_PROC*)get_proc("glDeleteProgram");
    glpfDeleteShader = (PFNGLDELETESHADER_PROC*)get_proc("glDeleteShader");
    glpfDetachShader = (PFNGLDETACHSHADER_PROC*)get_proc("glDetachShader");
    glpfDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAY_PROC*)get_proc("glDisableVertexAttribArray");
    glpfDrawBuffers = (PFNGLDRAWBUFFERS_PROC*)get_proc("glDrawBuffers");
    glpfEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAY_PROC*)get_proc("glEnableVertexAttribArray");
    glpfGetActiveAttrib = (PFNGLGETACTIVEATTRIB_PROC*)get_proc("glGetActiveAttrib");
    glpfGetActiveUniform = (PFNGLGETACTIVEUNIFORM_PROC*)get_proc("glGetActiveUniform");
    glpfGetAttachedShaders = (PFNGLGETATTACHEDSHADERS_PROC*)get_proc("glGetAttachedShaders");
    glpfGetAttribLocation = (PFNGLGETATTRIBLOCATION_PROC*)get_proc("glGetAttribLocation");
    glpfGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOG_PROC*)get_proc("glGetProgramInfoLog");
    glpfGetProgramiv = (PFNGLGETPROGRAMIV_PROC*)get_proc("glGetProgramiv");
    glpfGetShaderInfoLog = (PFNGLGETSHADERINFOLOG_PROC*)get_proc("glGetShaderInfoLog");
    glpfGetShaderSource = (PFNGLGETSHADERSOURCE_PROC*)get_proc("glGetShaderSource");
    glpfGetShaderiv = (PFNGLGETSHADERIV_PROC*)get_proc("glGetShaderiv");
    glpfGetUniformLocation = (PFNGLGETUNIFORMLOCATION_PROC*)get_proc("glGetUniformLocation");
    glpfGetUniformfv = (PFNGLGETUNIFORMFV_PROC*)get_proc("glGetUniformfv");
    glpfGetUniformiv = (PFNGLGETUNIFORMIV_PROC*)get_proc("glGetUniformiv");
    glpfGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERV_PROC*)get_proc("glGetVertexAttribPointerv");
    glpfGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDV_PROC*)get_proc("glGetVertexAttribdv");
    glpfGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFV_PROC*)get_proc("glGetVertexAttribfv");
    glpfGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIV_PROC*)get_proc("glGetVertexAttribiv");
    glpfIsProgram = (PFNGLISPROGRAM_PROC*)get_proc("glIsProgram");
    glpfIsShader = (PFNGLISSHADER_PROC*)get_proc("glIsShader");
    glpfLinkProgram = (PFNGLLINKPROGRAM_PROC*)get_proc("glLinkProgram");
    glpfShaderSource = (PFNGLSHADERSOURCE_PROC*)get_proc("glShaderSource");
    glpfStencilFuncSeparate = (PFNGLSTENCILFUNCSEPARATE_PROC*)get_proc("glStencilFuncSeparate");
    glpfStencilMaskSeparate = (PFNGLSTENCILMASKSEPARATE_PROC*)get_proc("glStencilMaskSeparate");
    glpfStencilOpSeparate = (PFNGLSTENCILOPSEPARATE_PROC*)get_proc("glStencilOpSeparate");
    glpfUniform1f = (PFNGLUNIFORM1F_PROC*)get_proc("glUniform1f");
    glpfUniform1fv = (PFNGLUNIFORM1FV_PROC*)get_proc("glUniform1fv");
    glpfUniform1i = (PFNGLUNIFORM1I_PROC*)get_proc("glUniform1i");
    glpfUniform1iv = (PFNGLUNIFORM1IV_PROC*)get_proc("glUniform1iv");
    glpfUniform2f = (PFNGLUNIFORM2F_PROC*)get_proc("glUniform2f");
    glpfUniform2fv = (PFNGLUNIFORM2FV_PROC*)get_proc("glUniform2fv");
    glpfUniform2i = (PFNGLUNIFORM2I_PROC*)get_proc("glUniform2i");
    glpfUniform2iv = (PFNGLUNIFORM2IV_PROC*)get_proc("glUniform2iv");
    glpfUniform3f = (PFNGLUNIFORM3F_PROC*)get_proc("glUniform3f");
    glpfUniform3fv = (PFNGLUNIFORM3FV_PROC*)get_proc("glUniform3fv");
    glpfUniform3i = (PFNGLUNIFORM3I_PROC*)get_proc("glUniform3i");
    glpfUniform3iv = (PFNGLUNIFORM3IV_PROC*)get_proc("glUniform3iv");
    glpfUniform4f = (PFNGLUNIFORM4F_PROC*)get_proc("glUniform4f");
    glpfUniform4fv = (PFNGLUNIFORM4FV_PROC*)get_proc("glUniform4fv");
    glpfUniform4i = (PFNGLUNIFORM4I_PROC*)get_proc("glUniform4i");
    glpfUniform4iv = (PFNGLUNIFORM4IV_PROC*)get_proc("glUniform4iv");
    glpfUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FV_PROC*)get_proc("glUniformMatrix2fv");
    glpfUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FV_PROC*)get_proc("glUniformMatrix3fv");
    glpfUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FV_PROC*)get_proc("glUniformMatrix4fv");
    glpfUseProgram = (PFNGLUSEPROGRAM_PROC*)get_proc("glUseProgram");
    glpfValidateProgram = (PFNGLVALIDATEPROGRAM_PROC*)get_proc("glValidateProgram");
    glpfVertexAttrib1d = (PFNGLVERTEXATTRIB1D_PROC*)get_proc("glVertexAttrib1d");
    glpfVertexAttrib1dv = (PFNGLVERTEXATTRIB1DV_PROC*)get_proc("glVertexAttrib1dv");
    glpfVertexAttrib1f = (PFNGLVERTEXATTRIB1F_PROC*)get_proc("glVertexAttrib1f");
    glpfVertexAttrib1fv = (PFNGLVERTEXATTRIB1FV_PROC*)get_proc("glVertexAttrib1fv");
    glpfVertexAttrib1s = (PFNGLVERTEXATTRIB1S_PROC*)get_proc("glVertexAttrib1s");
    glpfVertexAttrib1sv = (PFNGLVERTEXATTRIB1SV_PROC*)get_proc("glVertexAttrib1sv");
    glpfVertexAttrib2d = (PFNGLVERTEXATTRIB2D_PROC*)get_proc("glVertexAttrib2d");
    glpfVertexAttrib2dv = (PFNGLVERTEXATTRIB2DV_PROC*)get_proc("glVertexAttrib2dv");
    glpfVertexAttrib2f = (PFNGLVERTEXATTRIB2F_PROC*)get_proc("glVertexAttrib2f");
    glpfVertexAttrib2fv = (PFNGLVERTEXATTRIB2FV_PROC*)get_proc("glVertexAttrib2fv");
    glpfVertexAttrib2s = (PFNGLVERTEXATTRIB2S_PROC*)get_proc("glVertexAttrib2s");
    glpfVertexAttrib2sv = (PFNGLVERTEXATTRIB2SV_PROC*)get_proc("glVertexAttrib2sv");
    glpfVertexAttrib3d = (PFNGLVERTEXATTRIB3D_PROC*)get_proc("glVertexAttrib3d");
    glpfVertexAttrib3dv = (PFNGLVERTEXATTRIB3DV_PROC*)get_proc("glVertexAttrib3dv");
    glpfVertexAttrib3f = (PFNGLVERTEXATTRIB3F_PROC*)get_proc("glVertexAttrib3f");
    glpfVertexAttrib3fv = (PFNGLVERTEXATTRIB3FV_PROC*)get_proc("glVertexAttrib3fv");
    glpfVertexAttrib3s = (PFNGLVERTEXATTRIB3S_PROC*)get_proc("glVertexAttrib3s");
    glpfVertexAttrib3sv = (PFNGLVERTEXATTRIB3SV_PROC*)get_proc("glVertexAttrib3sv");
    glpfVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBV_PROC*)get_proc("glVertexAttrib4Nbv");
    glpfVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIV_PROC*)get_proc("glVertexAttrib4Niv");
    glpfVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSV_PROC*)get_proc("glVertexAttrib4Nsv");
    glpfVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUB_PROC*)get_proc("glVertexAttrib4Nub");
    glpfVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBV_PROC*)get_proc("glVertexAttrib4Nubv");
    glpfVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIV_PROC*)get_proc("glVertexAttrib4Nuiv");
    glpfVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSV_PROC*)get_proc("glVertexAttrib4Nusv");
    glpfVertexAttrib4bv = (PFNGLVERTEXATTRIB4BV_PROC*)get_proc("glVertexAttrib4bv");
    glpfVertexAttrib4d = (PFNGLVERTEXATTRIB4D_PROC*)get_proc("glVertexAttrib4d");
    glpfVertexAttrib4dv = (PFNGLVERTEXATTRIB4DV_PROC*)get_proc("glVertexAttrib4dv");
    glpfVertexAttrib4f = (PFNGLVERTEXATTRIB4F_PROC*)get_proc("glVertexAttrib4f");
    glpfVertexAttrib4fv = (PFNGLVERTEXATTRIB4FV_PROC*)get_proc("glVertexAttrib4fv");
    glpfVertexAttrib4iv = (PFNGLVERTEXATTRIB4IV_PROC*)get_proc("glVertexAttrib4iv");
    glpfVertexAttrib4s = (PFNGLVERTEXATTRIB4S_PROC*)get_proc("glVertexAttrib4s");
    glpfVertexAttrib4sv = (PFNGLVERTEXATTRIB4SV_PROC*)get_proc("glVertexAttrib4sv");
    glpfVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBV_PROC*)get_proc("glVertexAttrib4ubv");
    glpfVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIV_PROC*)get_proc("glVertexAttrib4uiv");
    glpfVertexAttrib4usv = (PFNGLVERTEXATTRIB4USV_PROC*)get_proc("glVertexAttrib4usv");
    glpfVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTER_PROC*)get_proc("glVertexAttribPointer");


    /* GL_VERSION_2_1 */

    glpfUniformMatrix2x3fv = (PFNGLUNIFORMMATRIX2X3FV_PROC*)get_proc("glUniformMatrix2x3fv");
    glpfUniformMatrix2x4fv = (PFNGLUNIFORMMATRIX2X4FV_PROC*)get_proc("glUniformMatrix2x4fv");
    glpfUniformMatrix3x2fv = (PFNGLUNIFORMMATRIX3X2FV_PROC*)get_proc("glUniformMatrix3x2fv");
    glpfUniformMatrix3x4fv = (PFNGLUNIFORMMATRIX3X4FV_PROC*)get_proc("glUniformMatrix3x4fv");
    glpfUniformMatrix4x2fv = (PFNGLUNIFORMMATRIX4X2FV_PROC*)get_proc("glUniformMatrix4x2fv");
    glpfUniformMatrix4x3fv = (PFNGLUNIFORMMATRIX4X3FV_PROC*)get_proc("glUniformMatrix4x3fv");


    /* GL_VERSION_3_0 */

    glpfBeginConditionalRender = (PFNGLBEGINCONDITIONALRENDER_PROC*)get_proc("glBeginConditionalRender");
    glpfBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACK_PROC*)get_proc("glBeginTransformFeedback");
    glpfBindBufferBase = (PFNGLBINDBUFFERBASE_PROC*)get_proc("glBindBufferBase");
    glpfBindBufferRange = (PFNGLBINDBUFFERRANGE_PROC*)get_proc("glBindBufferRange");
    glpfBindFragDataLocation = (PFNGLBINDFRAGDATALOCATION_PROC*)get_proc("glBindFragDataLocation");
    glpfBindFramebuffer = (PFNGLBINDFRAMEBUFFER_PROC*)get_proc("glBindFramebuffer");
    glpfBindRenderbuffer = (PFNGLBINDRENDERBUFFER_PROC*)get_proc("glBindRenderbuffer");
    glpfBindVertexArray = (PFNGLBINDVERTEXARRAY_PROC*)get_proc("glBindVertexArray");
    glpfBlitFramebuffer = (PFNGLBLITFRAMEBUFFER_PROC*)get_proc("glBlitFramebuffer");
    glpfCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUS_PROC*)get_proc("glCheckFramebufferStatus");
    glpfClampColor = (PFNGLCLAMPCOLOR_PROC*)get_proc("glClampColor");
    glpfClearBufferfi = (PFNGLCLEARBUFFERFI_PROC*)get_proc("glClearBufferfi");
    glpfClearBufferfv = (PFNGLCLEARBUFFERFV_PROC*)get_proc("glClearBufferfv");
    glpfClearBufferiv = (PFNGLCLEARBUFFERIV_PROC*)get_proc("glClearBufferiv");
    glpfClearBufferuiv = (PFNGLCLEARBUFFERUIV_PROC*)get_proc("glClearBufferuiv");
    glpfColorMaski = (PFNGLCOLORMASKI_PROC*)get_proc("glColorMaski");
    glpfDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERS_PROC*)get_proc("glDeleteFramebuffers");
    glpfDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERS_PROC*)get_proc("glDeleteRenderbuffers");
    glpfDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYS_PROC*)get_proc("glDeleteVertexArrays");
    glpfDisablei = (PFNGLDISABLEI_PROC*)get_proc("glDisablei");
    glpfEnablei = (PFNGLENABLEI_PROC*)get_proc("glEnablei");
    glpfEndConditionalRender = (PFNGLENDCONDITIONALRENDER_PROC*)get_proc("glEndConditionalRender");
    glpfEndTransformFeedback = (PFNGLENDTRANSFORMFEEDBACK_PROC*)get_proc("glEndTransformFeedback");
    glpfFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGE_PROC*)get_proc("glFlushMappedBufferRange");
    glpfFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFER_PROC*)get_proc("glFramebufferRenderbuffer");
    glpfFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1D_PROC*)get_proc("glFramebufferTexture1D");
    glpfFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2D_PROC*)get_proc("glFramebufferTexture2D");
    glpfFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3D_PROC*)get_proc("glFramebufferTexture3D");
    glpfFramebufferTextureLayer = (PFNGLFRAMEBUFFERTEXTURELAYER_PROC*)get_proc("glFramebufferTextureLayer");
    glpfGenFramebuffers = (PFNGLGENFRAMEBUFFERS_PROC*)get_proc("glGenFramebuffers");
    glpfGenRenderbuffers = (PFNGLGENRENDERBUFFERS_PROC*)get_proc("glGenRenderbuffers");
    glpfGenVertexArrays = (PFNGLGENVERTEXARRAYS_PROC*)get_proc("glGenVertexArrays");
    glpfGenerateMipmap = (PFNGLGENERATEMIPMAP_PROC*)get_proc("glGenerateMipmap");
    glpfGetBooleani_v = (PFNGLGETBOOLEANI_V_PROC*)get_proc("glGetBooleani_v");
    glpfGetFragDataLocation = (PFNGLGETFRAGDATALOCATION_PROC*)get_proc("glGetFragDataLocation");
    glpfGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIV_PROC*)get_proc("glGetFramebufferAttachmentParameteriv");
    glpfGetIntegeri_v = (PFNGLGETINTEGERI_V_PROC*)get_proc("glGetIntegeri_v");
    glpfGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIV_PROC*)get_proc("glGetRenderbufferParameteriv");
    glpfGetStringi = (PFNGLGETSTRINGI_PROC*)get_proc("glGetStringi");
    glpfGetTexParameterIiv = (PFNGLGETTEXPARAMETERIIV_PROC*)get_proc("glGetTexParameterIiv");
    glpfGetTexParameterIuiv = (PFNGLGETTEXPARAMETERIUIV_PROC*)get_proc("glGetTexParameterIuiv");
    glpfGetTransformFeedbackVarying = (PFNGLGETTRANSFORMFEEDBACKVARYING_PROC*)get_proc("glGetTransformFeedbackVarying");
    glpfGetUniformuiv = (PFNGLGETUNIFORMUIV_PROC*)get_proc("glGetUniformuiv");
    glpfGetVertexAttribIiv = (PFNGLGETVERTEXATTRIBIIV_PROC*)get_proc("glGetVertexAttribIiv");
    glpfGetVertexAttribIuiv = (PFNGLGETVERTEXATTRIBIUIV_PROC*)get_proc("glGetVertexAttribIuiv");
    glpfIsEnabledi = (PFNGLISENABLEDI_PROC*)get_proc("glIsEnabledi");
    glpfIsFramebuffer = (PFNGLISFRAMEBUFFER_PROC*)get_proc("glIsFramebuffer");
    glpfIsRenderbuffer = (PFNGLISRENDERBUFFER_PROC*)get_proc("glIsRenderbuffer");
    glpfIsVertexArray = (PFNGLISVERTEXARRAY_PROC*)get_proc("glIsVertexArray");
    glpfMapBufferRange = (PFNGLMAPBUFFERRANGE_PROC*)get_proc("glMapBufferRange");
    glpfRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGE_PROC*)get_proc("glRenderbufferStorage");
    glpfRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLE_PROC*)get_proc("glRenderbufferStorageMultisample");
    glpfTexParameterIiv = (PFNGLTEXPARAMETERIIV_PROC*)get_proc("glTexParameterIiv");
    glpfTexParameterIuiv = (PFNGLTEXPARAMETERIUIV_PROC*)get_proc("glTexParameterIuiv");
    glpfTransformFeedbackVaryings = (PFNGLTRANSFORMFEEDBACKVARYINGS_PROC*)get_proc("glTransformFeedbackVaryings");
    glpfUniform1ui = (PFNGLUNIFORM1UI_PROC*)get_proc("glUniform1ui");
    glpfUniform1uiv = (PFNGLUNIFORM1UIV_PROC*)get_proc("glUniform1uiv");
    glpfUniform2ui = (PFNGLUNIFORM2UI_PROC*)get_proc("glUniform2ui");
    glpfUniform2uiv = (PFNGLUNIFORM2UIV_PROC*)get_proc("glUniform2uiv");
    glpfUniform3ui = (PFNGLUNIFORM3UI_PROC*)get_proc("glUniform3ui");
    glpfUniform3uiv = (PFNGLUNIFORM3UIV_PROC*)get_proc("glUniform3uiv");
    glpfUniform4ui = (PFNGLUNIFORM4UI_PROC*)get_proc("glUniform4ui");
    glpfUniform4uiv = (PFNGLUNIFORM4UIV_PROC*)get_proc("glUniform4uiv");
    glpfVertexAttribI1i = (PFNGLVERTEXATTRIBI1I_PROC*)get_proc("glVertexAttribI1i");
    glpfVertexAttribI1iv = (PFNGLVERTEXATTRIBI1IV_PROC*)get_proc("glVertexAttribI1iv");
    glpfVertexAttribI1ui = (PFNGLVERTEXATTRIBI1UI_PROC*)get_proc("glVertexAttribI1ui");
    glpfVertexAttribI1uiv = (PFNGLVERTEXATTRIBI1UIV_PROC*)get_proc("glVertexAttribI1uiv");
    glpfVertexAttribI2i = (PFNGLVERTEXATTRIBI2I_PROC*)get_proc("glVertexAttribI2i");
    glpfVertexAttribI2iv = (PFNGLVERTEXATTRIBI2IV_PROC*)get_proc("glVertexAttribI2iv");
    glpfVertexAttribI2ui = (PFNGLVERTEXATTRIBI2UI_PROC*)get_proc("glVertexAttribI2ui");
    glpfVertexAttribI2uiv = (PFNGLVERTEXATTRIBI2UIV_PROC*)get_proc("glVertexAttribI2uiv");
    glpfVertexAttribI3i = (PFNGLVERTEXATTRIBI3I_PROC*)get_proc("glVertexAttribI3i");
    glpfVertexAttribI3iv = (PFNGLVERTEXATTRIBI3IV_PROC*)get_proc("glVertexAttribI3iv");
    glpfVertexAttribI3ui = (PFNGLVERTEXATTRIBI3UI_PROC*)get_proc("glVertexAttribI3ui");
    glpfVertexAttribI3uiv = (PFNGLVERTEXATTRIBI3UIV_PROC*)get_proc("glVertexAttribI3uiv");
    glpfVertexAttribI4bv = (PFNGLVERTEXATTRIBI4BV_PROC*)get_proc("glVertexAttribI4bv");
    glpfVertexAttribI4i = (PFNGLVERTEXATTRIBI4I_PROC*)get_proc("glVertexAttribI4i");
    glpfVertexAttribI4iv = (PFNGLVERTEXATTRIBI4IV_PROC*)get_proc("glVertexAttribI4iv");
    glpfVertexAttribI4sv = (PFNGLVERTEXATTRIBI4SV_PROC*)get_proc("glVertexAttribI4sv");
    glpfVertexAttribI4ubv = (PFNGLVERTEXATTRIBI4UBV_PROC*)get_proc("glVertexAttribI4ubv");
    glpfVertexAttribI4ui = (PFNGLVERTEXATTRIBI4UI_PROC*)get_proc("glVertexAttribI4ui");
    glpfVertexAttribI4uiv = (PFNGLVERTEXATTRIBI4UIV_PROC*)get_proc("glVertexAttribI4uiv");
    glpfVertexAttribI4usv = (PFNGLVERTEXATTRIBI4USV_PROC*)get_proc("glVertexAttribI4usv");
    glpfVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTER_PROC*)get_proc("glVertexAttribIPointer");


    /* GL_VERSION_3_1 */

    glpfCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATA_PROC*)get_proc("glCopyBufferSubData");
    glpfDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCED_PROC*)get_proc("glDrawArraysInstanced");
    glpfDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCED_PROC*)get_proc("glDrawElementsInstanced");
    glpfGetActiveUniformBlockName = (PFNGLGETACTIVEUNIFORMBLOCKNAME_PROC*)get_proc("glGetActiveUniformBlockName");
    glpfGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIV_PROC*)get_proc("glGetActiveUniformBlockiv");
    glpfGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAME_PROC*)get_proc("glGetActiveUniformName");
    glpfGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIV_PROC*)get_proc("glGetActiveUniformsiv");
    glpfGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEX_PROC*)get_proc("glGetUniformBlockIndex");
    glpfGetUniformIndices = (PFNGLGETUNIFORMINDICES_PROC*)get_proc("glGetUniformIndices");
    glpfPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEX_PROC*)get_proc("glPrimitiveRestartIndex");
    glpfTexBuffer = (PFNGLTEXBUFFER_PROC*)get_proc("glTexBuffer");
    glpfUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDING_PROC*)get_proc("glUniformBlockBinding");


    /* GL_VERSION_3_2 */

    glpfClientWaitSync = (PFNGLCLIENTWAITSYNC_PROC*)get_proc("glClientWaitSync");
    glpfDeleteSync = (PFNGLDELETESYNC_PROC*)get_proc("glDeleteSync");
    glpfDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEX_PROC*)get_proc("glDrawElementsBaseVertex");
    glpfDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEX_PROC*)get_proc("glDrawElementsInstancedBaseVertex");
    glpfDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEX_PROC*)get_proc("glDrawRangeElementsBaseVertex");
    glpfFenceSync = (PFNGLFENCESYNC_PROC*)get_proc("glFenceSync");
    glpfFramebufferTexture = (PFNGLFRAMEBUFFERTEXTURE_PROC*)get_proc("glFramebufferTexture");
    glpfGetBufferParameteri64v = (PFNGLGETBUFFERPARAMETERI64V_PROC*)get_proc("glGetBufferParameteri64v");
    glpfGetInteger64i_v = (PFNGLGETINTEGER64I_V_PROC*)get_proc("glGetInteger64i_v");
    glpfGetInteger64v = (PFNGLGETINTEGER64V_PROC*)get_proc("glGetInteger64v");
    glpfGetMultisamplefv = (PFNGLGETMULTISAMPLEFV_PROC*)get_proc("glGetMultisamplefv");
    glpfGetSynciv = (PFNGLGETSYNCIV_PROC*)get_proc("glGetSynciv");
    glpfIsSync = (PFNGLISSYNC_PROC*)get_proc("glIsSync");
    glpfMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEX_PROC*)get_proc("glMultiDrawElementsBaseVertex");
    glpfProvokingVertex = (PFNGLPROVOKINGVERTEX_PROC*)get_proc("glProvokingVertex");
    glpfSampleMaski = (PFNGLSAMPLEMASKI_PROC*)get_proc("glSampleMaski");
    glpfTexImage2DMultisample = (PFNGLTEXIMAGE2DMULTISAMPLE_PROC*)get_proc("glTexImage2DMultisample");
    glpfTexImage3DMultisample = (PFNGLTEXIMAGE3DMULTISAMPLE_PROC*)get_proc("glTexImage3DMultisample");
    glpfWaitSync = (PFNGLWAITSYNC_PROC*)get_proc("glWaitSync");


    /* GL_VERSION_3_3 */

    glpfBindFragDataLocationIndexed = (PFNGLBINDFRAGDATALOCATIONINDEXED_PROC*)get_proc("glBindFragDataLocationIndexed");
    glpfBindSampler = (PFNGLBINDSAMPLER_PROC*)get_proc("glBindSampler");
    glpfDeleteSamplers = (PFNGLDELETESAMPLERS_PROC*)get_proc("glDeleteSamplers");
    glpfGenSamplers = (PFNGLGENSAMPLERS_PROC*)get_proc("glGenSamplers");
    glpfGetFragDataIndex = (PFNGLGETFRAGDATAINDEX_PROC*)get_proc("glGetFragDataIndex");
    glpfGetQueryObjecti64v = (PFNGLGETQUERYOBJECTI64V_PROC*)get_proc("glGetQueryObjecti64v");
    glpfGetQueryObjectui64v = (PFNGLGETQUERYOBJECTUI64V_PROC*)get_proc("glGetQueryObjectui64v");
    glpfGetSamplerParameterIiv = (PFNGLGETSAMPLERPARAMETERIIV_PROC*)get_proc("glGetSamplerParameterIiv");
    glpfGetSamplerParameterIuiv = (PFNGLGETSAMPLERPARAMETERIUIV_PROC*)get_proc("glGetSamplerParameterIuiv");
    glpfGetSamplerParameterfv = (PFNGLGETSAMPLERPARAMETERFV_PROC*)get_proc("glGetSamplerParameterfv");
    glpfGetSamplerParameteriv = (PFNGLGETSAMPLERPARAMETERIV_PROC*)get_proc("glGetSamplerParameteriv");
    glpfIsSampler = (PFNGLISSAMPLER_PROC*)get_proc("glIsSampler");
    glpfQueryCounter = (PFNGLQUERYCOUNTER_PROC*)get_proc("glQueryCounter");
    glpfSamplerParameterIiv = (PFNGLSAMPLERPARAMETERIIV_PROC*)get_proc("glSamplerParameterIiv");
    glpfSamplerParameterIuiv = (PFNGLSAMPLERPARAMETERIUIV_PROC*)get_proc("glSamplerParameterIuiv");
    glpfSamplerParameterf = (PFNGLSAMPLERPARAMETERF_PROC*)get_proc("glSamplerParameterf");
    glpfSamplerParameterfv = (PFNGLSAMPLERPARAMETERFV_PROC*)get_proc("glSamplerParameterfv");
    glpfSamplerParameteri = (PFNGLSAMPLERPARAMETERI_PROC*)get_proc("glSamplerParameteri");
    glpfSamplerParameteriv = (PFNGLSAMPLERPARAMETERIV_PROC*)get_proc("glSamplerParameteriv");
    glpfVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISOR_PROC*)get_proc("glVertexAttribDivisor");
    glpfVertexAttribP1ui = (PFNGLVERTEXATTRIBP1UI_PROC*)get_proc("glVertexAttribP1ui");
    glpfVertexAttribP1uiv = (PFNGLVERTEXATTRIBP1UIV_PROC*)get_proc("glVertexAttribP1uiv");
    glpfVertexAttribP2ui = (PFNGLVERTEXATTRIBP2UI_PROC*)get_proc("glVertexAttribP2ui");
    glpfVertexAttribP2uiv = (PFNGLVERTEXATTRIBP2UIV_PROC*)get_proc("glVertexAttribP2uiv");
    glpfVertexAttribP3ui = (PFNGLVERTEXATTRIBP3UI_PROC*)get_proc("glVertexAttribP3ui");
    glpfVertexAttribP3uiv = (PFNGLVERTEXATTRIBP3UIV_PROC*)get_proc("glVertexAttribP3uiv");
    glpfVertexAttribP4ui = (PFNGLVERTEXATTRIBP4UI_PROC*)get_proc("glVertexAttribP4ui");
    glpfVertexAttribP4uiv = (PFNGLVERTEXATTRIBP4UIV_PROC*)get_proc("glVertexAttribP4uiv");


    /* GL_VERSION_4_0 */

    glpfBeginQueryIndexed = (PFNGLBEGINQUERYINDEXED_PROC*)get_proc("glBeginQueryIndexed");
    glpfBindTransformFeedback = (PFNGLBINDTRANSFORMFEEDBACK_PROC*)get_proc("glBindTransformFeedback");
    glpfBlendEquationSeparatei = (PFNGLBLENDEQUATIONSEPARATEI_PROC*)get_proc("glBlendEquationSeparatei");
    glpfBlendEquationi = (PFNGLBLENDEQUATIONI_PROC*)get_proc("glBlendEquationi");
    glpfBlendFuncSeparatei = (PFNGLBLENDFUNCSEPARATEI_PROC*)get_proc("glBlendFuncSeparatei");
    glpfBlendFunci = (PFNGLBLENDFUNCI_PROC*)get_proc("glBlendFunci");
    glpfDeleteTransformFeedbacks = (PFNGLDELETETRANSFORMFEEDBACKS_PROC*)get_proc("glDeleteTransformFeedbacks");
    glpfDrawArraysIndirect = (PFNGLDRAWARRAYSINDIRECT_PROC*)get_proc("glDrawArraysIndirect");
    glpfDrawElementsIndirect = (PFNGLDRAWELEMENTSINDIRECT_PROC*)get_proc("glDrawElementsIndirect");
    glpfDrawTransformFeedback = (PFNGLDRAWTRANSFORMFEEDBACK_PROC*)get_proc("glDrawTransformFeedback");
    glpfDrawTransformFeedbackStream = (PFNGLDRAWTRANSFORMFEEDBACKSTREAM_PROC*)get_proc("glDrawTransformFeedbackStream");
    glpfEndQueryIndexed = (PFNGLENDQUERYINDEXED_PROC*)get_proc("glEndQueryIndexed");
    glpfGenTransformFeedbacks = (PFNGLGENTRANSFORMFEEDBACKS_PROC*)get_proc("glGenTransformFeedbacks");
    glpfGetActiveSubroutineName = (PFNGLGETACTIVESUBROUTINENAME_PROC*)get_proc("glGetActiveSubroutineName");
    glpfGetActiveSubroutineUniformName = (PFNGLGETACTIVESUBROUTINEUNIFORMNAME_PROC*)get_proc("glGetActiveSubroutineUniformName");
    glpfGetActiveSubroutineUniformiv = (PFNGLGETACTIVESUBROUTINEUNIFORMIV_PROC*)get_proc("glGetActiveSubroutineUniformiv");
    glpfGetProgramStageiv = (PFNGLGETPROGRAMSTAGEIV_PROC*)get_proc("glGetProgramStageiv");
    glpfGetQueryIndexediv = (PFNGLGETQUERYINDEXEDIV_PROC*)get_proc("glGetQueryIndexediv");
    glpfGetSubroutineIndex = (PFNGLGETSUBROUTINEINDEX_PROC*)get_proc("glGetSubroutineIndex");
    glpfGetSubroutineUniformLocation = (PFNGLGETSUBROUTINEUNIFORMLOCATION_PROC*)get_proc("glGetSubroutineUniformLocation");
    glpfGetUniformSubroutineuiv = (PFNGLGETUNIFORMSUBROUTINEUIV_PROC*)get_proc("glGetUniformSubroutineuiv");
    glpfGetUniformdv = (PFNGLGETUNIFORMDV_PROC*)get_proc("glGetUniformdv");
    glpfIsTransformFeedback = (PFNGLISTRANSFORMFEEDBACK_PROC*)get_proc("glIsTransformFeedback");
    glpfMinSampleShading = (PFNGLMINSAMPLESHADING_PROC*)get_proc("glMinSampleShading");
    glpfPatchParameterfv = (PFNGLPATCHPARAMETERFV_PROC*)get_proc("glPatchParameterfv");
    glpfPatchParameteri = (PFNGLPATCHPARAMETERI_PROC*)get_proc("glPatchParameteri");
    glpfPauseTransformFeedback = (PFNGLPAUSETRANSFORMFEEDBACK_PROC*)get_proc("glPauseTransformFeedback");
    glpfResumeTransformFeedback = (PFNGLRESUMETRANSFORMFEEDBACK_PROC*)get_proc("glResumeTransformFeedback");
    glpfUniform1d = (PFNGLUNIFORM1D_PROC*)get_proc("glUniform1d");
    glpfUniform1dv = (PFNGLUNIFORM1DV_PROC*)get_proc("glUniform1dv");
    glpfUniform2d = (PFNGLUNIFORM2D_PROC*)get_proc("glUniform2d");
    glpfUniform2dv = (PFNGLUNIFORM2DV_PROC*)get_proc("glUniform2dv");
    glpfUniform3d = (PFNGLUNIFORM3D_PROC*)get_proc("glUniform3d");
    glpfUniform3dv = (PFNGLUNIFORM3DV_PROC*)get_proc("glUniform3dv");
    glpfUniform4d = (PFNGLUNIFORM4D_PROC*)get_proc("glUniform4d");
    glpfUniform4dv = (PFNGLUNIFORM4DV_PROC*)get_proc("glUniform4dv");
    glpfUniformMatrix2dv = (PFNGLUNIFORMMATRIX2DV_PROC*)get_proc("glUniformMatrix2dv");
    glpfUniformMatrix2x3dv = (PFNGLUNIFORMMATRIX2X3DV_PROC*)get_proc("glUniformMatrix2x3dv");
    glpfUniformMatrix2x4dv = (PFNGLUNIFORMMATRIX2X4DV_PROC*)get_proc("glUniformMatrix2x4dv");
    glpfUniformMatrix3dv = (PFNGLUNIFORMMATRIX3DV_PROC*)get_proc("glUniformMatrix3dv");
    glpfUniformMatrix3x2dv = (PFNGLUNIFORMMATRIX3X2DV_PROC*)get_proc("glUniformMatrix3x2dv");
    glpfUniformMatrix3x4dv = (PFNGLUNIFORMMATRIX3X4DV_PROC*)get_proc("glUniformMatrix3x4dv");
    glpfUniformMatrix4dv = (PFNGLUNIFORMMATRIX4DV_PROC*)get_proc("glUniformMatrix4dv");
    glpfUniformMatrix4x2dv = (PFNGLUNIFORMMATRIX4X2DV_PROC*)get_proc("glUniformMatrix4x2dv");
    glpfUniformMatrix4x3dv = (PFNGLUNIFORMMATRIX4X3DV_PROC*)get_proc("glUniformMatrix4x3dv");
    glpfUniformSubroutinesuiv = (PFNGLUNIFORMSUBROUTINESUIV_PROC*)get_proc("glUniformSubroutinesuiv");


    /* GL_VERSION_4_1 */

    glpfActiveShaderProgram = (PFNGLACTIVESHADERPROGRAM_PROC*)get_proc("glActiveShaderProgram");
    glpfBindProgramPipeline = (PFNGLBINDPROGRAMPIPELINE_PROC*)get_proc("glBindProgramPipeline");
    glpfClearDepthf = (PFNGLCLEARDEPTHF_PROC*)get_proc("glClearDepthf");
    glpfCreateShaderProgramv = (PFNGLCREATESHADERPROGRAMV_PROC*)get_proc("glCreateShaderProgramv");
    glpfDeleteProgramPipelines = (PFNGLDELETEPROGRAMPIPELINES_PROC*)get_proc("glDeleteProgramPipelines");
    glpfDepthRangeArrayv = (PFNGLDEPTHRANGEARRAYV_PROC*)get_proc("glDepthRangeArrayv");
    glpfDepthRangeIndexed = (PFNGLDEPTHRANGEINDEXED_PROC*)get_proc("glDepthRangeIndexed");
    glpfDepthRangef = (PFNGLDEPTHRANGEF_PROC*)get_proc("glDepthRangef");
    glpfGenProgramPipelines = (PFNGLGENPROGRAMPIPELINES_PROC*)get_proc("glGenProgramPipelines");
    glpfGetDoublei_v = (PFNGLGETDOUBLEI_V_PROC*)get_proc("glGetDoublei_v");
    glpfGetFloati_v = (PFNGLGETFLOATI_V_PROC*)get_proc("glGetFloati_v");
    glpfGetProgramBinary = (PFNGLGETPROGRAMBINARY_PROC*)get_proc("glGetProgramBinary");
    glpfGetProgramPipelineInfoLog = (PFNGLGETPROGRAMPIPELINEINFOLOG_PROC*)get_proc("glGetProgramPipelineInfoLog");
    glpfGetProgramPipelineiv = (PFNGLGETPROGRAMPIPELINEIV_PROC*)get_proc("glGetProgramPipelineiv");
    glpfGetShaderPrecisionFormat = (PFNGLGETSHADERPRECISIONFORMAT_PROC*)get_proc("glGetShaderPrecisionFormat");
    glpfGetVertexAttribLdv = (PFNGLGETVERTEXATTRIBLDV_PROC*)get_proc("glGetVertexAttribLdv");
    glpfIsProgramPipeline = (PFNGLISPROGRAMPIPELINE_PROC*)get_proc("glIsProgramPipeline");
    glpfProgramBinary = (PFNGLPROGRAMBINARY_PROC*)get_proc("glProgramBinary");
    glpfProgramParameteri = (PFNGLPROGRAMPARAMETERI_PROC*)get_proc("glProgramParameteri");
    glpfProgramUniform1d = (PFNGLPROGRAMUNIFORM1D_PROC*)get_proc("glProgramUniform1d");
    glpfProgramUniform1dv = (PFNGLPROGRAMUNIFORM1DV_PROC*)get_proc("glProgramUniform1dv");
    glpfProgramUniform1f = (PFNGLPROGRAMUNIFORM1F_PROC*)get_proc("glProgramUniform1f");
    glpfProgramUniform1fv = (PFNGLPROGRAMUNIFORM1FV_PROC*)get_proc("glProgramUniform1fv");
    glpfProgramUniform1i = (PFNGLPROGRAMUNIFORM1I_PROC*)get_proc("glProgramUniform1i");
    glpfProgramUniform1iv = (PFNGLPROGRAMUNIFORM1IV_PROC*)get_proc("glProgramUniform1iv");
    glpfProgramUniform1ui = (PFNGLPROGRAMUNIFORM1UI_PROC*)get_proc("glProgramUniform1ui");
    glpfProgramUniform1uiv = (PFNGLPROGRAMUNIFORM1UIV_PROC*)get_proc("glProgramUniform1uiv");
    glpfProgramUniform2d = (PFNGLPROGRAMUNIFORM2D_PROC*)get_proc("glProgramUniform2d");
    glpfProgramUniform2dv = (PFNGLPROGRAMUNIFORM2DV_PROC*)get_proc("glProgramUniform2dv");
    glpfProgramUniform2f = (PFNGLPROGRAMUNIFORM2F_PROC*)get_proc("glProgramUniform2f");
    glpfProgramUniform2fv = (PFNGLPROGRAMUNIFORM2FV_PROC*)get_proc("glProgramUniform2fv");
    glpfProgramUniform2i = (PFNGLPROGRAMUNIFORM2I_PROC*)get_proc("glProgramUniform2i");
    glpfProgramUniform2iv = (PFNGLPROGRAMUNIFORM2IV_PROC*)get_proc("glProgramUniform2iv");
    glpfProgramUniform2ui = (PFNGLPROGRAMUNIFORM2UI_PROC*)get_proc("glProgramUniform2ui");
    glpfProgramUniform2uiv = (PFNGLPROGRAMUNIFORM2UIV_PROC*)get_proc("glProgramUniform2uiv");
    glpfProgramUniform3d = (PFNGLPROGRAMUNIFORM3D_PROC*)get_proc("glProgramUniform3d");
    glpfProgramUniform3dv = (PFNGLPROGRAMUNIFORM3DV_PROC*)get_proc("glProgramUniform3dv");
    glpfProgramUniform3f = (PFNGLPROGRAMUNIFORM3F_PROC*)get_proc("glProgramUniform3f");
    glpfProgramUniform3fv = (PFNGLPROGRAMUNIFORM3FV_PROC*)get_proc("glProgramUniform3fv");
    glpfProgramUniform3i = (PFNGLPROGRAMUNIFORM3I_PROC*)get_proc("glProgramUniform3i");
    glpfProgramUniform3iv = (PFNGLPROGRAMUNIFORM3IV_PROC*)get_proc("glProgramUniform3iv");
    glpfProgramUniform3ui = (PFNGLPROGRAMUNIFORM3UI_PROC*)get_proc("glProgramUniform3ui");
    glpfProgramUniform3uiv = (PFNGLPROGRAMUNIFORM3UIV_PROC*)get_proc("glProgramUniform3uiv");
    glpfProgramUniform4d = (PFNGLPROGRAMUNIFORM4D_PROC*)get_proc("glProgramUniform4d");
    glpfProgramUniform4dv = (PFNGLPROGRAMUNIFORM4DV_PROC*)get_proc("glProgramUniform4dv");
    glpfProgramUniform4f = (PFNGLPROGRAMUNIFORM4F_PROC*)get_proc("glProgramUniform4f");
    glpfProgramUniform4fv = (PFNGLPROGRAMUNIFORM4FV_PROC*)get_proc("glProgramUniform4fv");
    glpfProgramUniform4i = (PFNGLPROGRAMUNIFORM4I_PROC*)get_proc("glProgramUniform4i");
    glpfProgramUniform4iv = (PFNGLPROGRAMUNIFORM4IV_PROC*)get_proc("glProgramUniform4iv");
    glpfProgramUniform4ui = (PFNGLPROGRAMUNIFORM4UI_PROC*)get_proc("glProgramUniform4ui");
    glpfProgramUniform4uiv = (PFNGLPROGRAMUNIFORM4UIV_PROC*)get_proc("glProgramUniform4uiv");
    glpfProgramUniformMatrix2dv = (PFNGLPROGRAMUNIFORMMATRIX2DV_PROC*)get_proc("glProgramUniformMatrix2dv");
    glpfProgramUniformMatrix2fv = (PFNGLPROGRAMUNIFORMMATRIX2FV_PROC*)get_proc("glProgramUniformMatrix2fv");
    glpfProgramUniformMatrix2x3dv = (PFNGLPROGRAMUNIFORMMATRIX2X3DV_PROC*)get_proc("glProgramUniformMatrix2x3dv");
    glpfProgramUniformMatrix2x3fv = (PFNGLPROGRAMUNIFORMMATRIX2X3FV_PROC*)get_proc("glProgramUniformMatrix2x3fv");
    glpfProgramUniformMatrix2x4dv = (PFNGLPROGRAMUNIFORMMATRIX2X4DV_PROC*)get_proc("glProgramUniformMatrix2x4dv");
    glpfProgramUniformMatrix2x4fv = (PFNGLPROGRAMUNIFORMMATRIX2X4FV_PROC*)get_proc("glProgramUniformMatrix2x4fv");
    glpfProgramUniformMatrix3dv = (PFNGLPROGRAMUNIFORMMATRIX3DV_PROC*)get_proc("glProgramUniformMatrix3dv");
    glpfProgramUniformMatrix3fv = (PFNGLPROGRAMUNIFORMMATRIX3FV_PROC*)get_proc("glProgramUniformMatrix3fv");
    glpfProgramUniformMatrix3x2dv = (PFNGLPROGRAMUNIFORMMATRIX3X2DV_PROC*)get_proc("glProgramUniformMatrix3x2dv");
    glpfProgramUniformMatrix3x2fv = (PFNGLPROGRAMUNIFORMMATRIX3X2FV_PROC*)get_proc("glProgramUniformMatrix3x2fv");
    glpfProgramUniformMatrix3x4dv = (PFNGLPROGRAMUNIFORMMATRIX3X4DV_PROC*)get_proc("glProgramUniformMatrix3x4dv");
    glpfProgramUniformMatrix3x4fv = (PFNGLPROGRAMUNIFORMMATRIX3X4FV_PROC*)get_proc("glProgramUniformMatrix3x4fv");
    glpfProgramUniformMatrix4dv = (PFNGLPROGRAMUNIFORMMATRIX4DV_PROC*)get_proc("glProgramUniformMatrix4dv");
    glpfProgramUniformMatrix4fv = (PFNGLPROGRAMUNIFORMMATRIX4FV_PROC*)get_proc("glProgramUniformMatrix4fv");
    glpfProgramUniformMatrix4x2dv = (PFNGLPROGRAMUNIFORMMATRIX4X2DV_PROC*)get_proc("glProgramUniformMatrix4x2dv");
    glpfProgramUniformMatrix4x2fv = (PFNGLPROGRAMUNIFORMMATRIX4X2FV_PROC*)get_proc("glProgramUniformMatrix4x2fv");
    glpfProgramUniformMatrix4x3dv = (PFNGLPROGRAMUNIFORMMATRIX4X3DV_PROC*)get_proc("glProgramUniformMatrix4x3dv");
    glpfProgramUniformMatrix4x3fv = (PFNGLPROGRAMUNIFORMMATRIX4X3FV_PROC*)get_proc("glProgramUniformMatrix4x3fv");
    glpfReleaseShaderCompiler = (PFNGLRELEASESHADERCOMPILER_PROC*)get_proc("glReleaseShaderCompiler");
    glpfScissorArrayv = (PFNGLSCISSORARRAYV_PROC*)get_proc("glScissorArrayv");
    glpfScissorIndexed = (PFNGLSCISSORINDEXED_PROC*)get_proc("glScissorIndexed");
    glpfScissorIndexedv = (PFNGLSCISSORINDEXEDV_PROC*)get_proc("glScissorIndexedv");
    glpfShaderBinary = (PFNGLSHADERBINARY_PROC*)get_proc("glShaderBinary");
    glpfUseProgramStages = (PFNGLUSEPROGRAMSTAGES_PROC*)get_proc("glUseProgramStages");
    glpfValidateProgramPipeline = (PFNGLVALIDATEPROGRAMPIPELINE_PROC*)get_proc("glValidateProgramPipeline");
    glpfVertexAttribL1d = (PFNGLVERTEXATTRIBL1D_PROC*)get_proc("glVertexAttribL1d");
    glpfVertexAttribL1dv = (PFNGLVERTEXATTRIBL1DV_PROC*)get_proc("glVertexAttribL1dv");
    glpfVertexAttribL2d = (PFNGLVERTEXATTRIBL2D_PROC*)get_proc("glVertexAttribL2d");
    glpfVertexAttribL2dv = (PFNGLVERTEXATTRIBL2DV_PROC*)get_proc("glVertexAttribL2dv");
    glpfVertexAttribL3d = (PFNGLVERTEXATTRIBL3D_PROC*)get_proc("glVertexAttribL3d");
    glpfVertexAttribL3dv = (PFNGLVERTEXATTRIBL3DV_PROC*)get_proc("glVertexAttribL3dv");
    glpfVertexAttribL4d = (PFNGLVERTEXATTRIBL4D_PROC*)get_proc("glVertexAttribL4d");
    glpfVertexAttribL4dv = (PFNGLVERTEXATTRIBL4DV_PROC*)get_proc("glVertexAttribL4dv");
    glpfVertexAttribLPointer = (PFNGLVERTEXATTRIBLPOINTER_PROC*)get_proc("glVertexAttribLPointer");
    glpfViewportArrayv = (PFNGLVIEWPORTARRAYV_PROC*)get_proc("glViewportArrayv");
    glpfViewportIndexedf = (PFNGLVIEWPORTINDEXEDF_PROC*)get_proc("glViewportIndexedf");
    glpfViewportIndexedfv = (PFNGLVIEWPORTINDEXEDFV_PROC*)get_proc("glViewportIndexedfv");


    /* GL_VERSION_4_2 */

    glpfBindImageTexture = (PFNGLBINDIMAGETEXTURE_PROC*)get_proc("glBindImageTexture");
    glpfDrawArraysInstancedBaseInstance = (PFNGLDRAWARRAYSINSTANCEDBASEINSTANCE_PROC*)get_proc("glDrawArraysInstancedBaseInstance");
    glpfDrawElementsInstancedBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCE_PROC*)get_proc("glDrawElementsInstancedBaseInstance");
    glpfDrawElementsInstancedBaseVertexBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCE_PROC*)get_proc("glDrawElementsInstancedBaseVertexBaseInstance");
    glpfDrawTransformFeedbackInstanced = (PFNGLDRAWTRANSFORMFEEDBACKINSTANCED_PROC*)get_proc("glDrawTransformFeedbackInstanced");
    glpfDrawTransformFeedbackStreamInstanced = (PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCED_PROC*)get_proc("glDrawTransformFeedbackStreamInstanced");
    glpfGetActiveAtomicCounterBufferiv = (PFNGLGETACTIVEATOMICCOUNTERBUFFERIV_PROC*)get_proc("glGetActiveAtomicCounterBufferiv");
    glpfGetInternalformativ = (PFNGLGETINTERNALFORMATIV_PROC*)get_proc("glGetInternalformativ");
    glpfMemoryBarrier = (PFNGLMEMORYBARRIER_PROC*)get_proc("glMemoryBarrier");
    glpfTexStorage1D = (PFNGLTEXSTORAGE1D_PROC*)get_proc("glTexStorage1D");
    glpfTexStorage2D = (PFNGLTEXSTORAGE2D_PROC*)get_proc("glTexStorage2D");
    glpfTexStorage3D = (PFNGLTEXSTORAGE3D_PROC*)get_proc("glTexStorage3D");


    /* GL_VERSION_4_3 */

    glpfBindVertexBuffer = (PFNGLBINDVERTEXBUFFER_PROC*)get_proc("glBindVertexBuffer");
    glpfClearBufferData = (PFNGLCLEARBUFFERDATA_PROC*)get_proc("glClearBufferData");
    glpfClearBufferSubData = (PFNGLCLEARBUFFERSUBDATA_PROC*)get_proc("glClearBufferSubData");
    glpfCopyImageSubData = (PFNGLCOPYIMAGESUBDATA_PROC*)get_proc("glCopyImageSubData");
    glpfDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACK_PROC*)get_proc("glDebugMessageCallback");
    glpfDebugMessageControl = (PFNGLDEBUGMESSAGECONTROL_PROC*)get_proc("glDebugMessageControl");
    glpfDebugMessageInsert = (PFNGLDEBUGMESSAGEINSERT_PROC*)get_proc("glDebugMessageInsert");
    glpfDispatchCompute = (PFNGLDISPATCHCOMPUTE_PROC*)get_proc("glDispatchCompute");
    glpfDispatchComputeIndirect = (PFNGLDISPATCHCOMPUTEINDIRECT_PROC*)get_proc("glDispatchComputeIndirect");
    glpfFramebufferParameteri = (PFNGLFRAMEBUFFERPARAMETERI_PROC*)get_proc("glFramebufferParameteri");
    glpfGetDebugMessageLog = (PFNGLGETDEBUGMESSAGELOG_PROC*)get_proc("glGetDebugMessageLog");
    glpfGetFramebufferParameteriv = (PFNGLGETFRAMEBUFFERPARAMETERIV_PROC*)get_proc("glGetFramebufferParameteriv");
    glpfGetInternalformati64v = (PFNGLGETINTERNALFORMATI64V_PROC*)get_proc("glGetInternalformati64v");
    glpfGetObjectLabel = (PFNGLGETOBJECTLABEL_PROC*)get_proc("glGetObjectLabel");
    glpfGetObjectPtrLabel = (PFNGLGETOBJECTPTRLABEL_PROC*)get_proc("glGetObjectPtrLabel");
    glpfGetPointerv = (PFNGLGETPOINTERV_PROC*)get_proc("glGetPointerv");
    glpfGetProgramInterfaceiv = (PFNGLGETPROGRAMINTERFACEIV_PROC*)get_proc("glGetProgramInterfaceiv");
    glpfGetProgramResourceIndex = (PFNGLGETPROGRAMRESOURCEINDEX_PROC*)get_proc("glGetProgramResourceIndex");
    glpfGetProgramResourceLocation = (PFNGLGETPROGRAMRESOURCELOCATION_PROC*)get_proc("glGetProgramResourceLocation");
    glpfGetProgramResourceLocationIndex = (PFNGLGETPROGRAMRESOURCELOCATIONINDEX_PROC*)get_proc("glGetProgramResourceLocationIndex");
    glpfGetProgramResourceName = (PFNGLGETPROGRAMRESOURCENAME_PROC*)get_proc("glGetProgramResourceName");
    glpfGetProgramResourceiv = (PFNGLGETPROGRAMRESOURCEIV_PROC*)get_proc("glGetProgramResourceiv");
    glpfInvalidateBufferData = (PFNGLINVALIDATEBUFFERDATA_PROC*)get_proc("glInvalidateBufferData");
    glpfInvalidateBufferSubData = (PFNGLINVALIDATEBUFFERSUBDATA_PROC*)get_proc("glInvalidateBufferSubData");
    glpfInvalidateFramebuffer = (PFNGLINVALIDATEFRAMEBUFFER_PROC*)get_proc("glInvalidateFramebuffer");
    glpfInvalidateSubFramebuffer = (PFNGLINVALIDATESUBFRAMEBUFFER_PROC*)get_proc("glInvalidateSubFramebuffer");
    glpfInvalidateTexImage = (PFNGLINVALIDATETEXIMAGE_PROC*)get_proc("glInvalidateTexImage");
    glpfInvalidateTexSubImage = (PFNGLINVALIDATETEXSUBIMAGE_PROC*)get_proc("glInvalidateTexSubImage");
    glpfMultiDrawArraysIndirect = (PFNGLMULTIDRAWARRAYSINDIRECT_PROC*)get_proc("glMultiDrawArraysIndirect");
    glpfMultiDrawElementsIndirect = (PFNGLMULTIDRAWELEMENTSINDIRECT_PROC*)get_proc("glMultiDrawElementsIndirect");
    glpfObjectLabel = (PFNGLOBJECTLABEL_PROC*)get_proc("glObjectLabel");
    glpfObjectPtrLabel = (PFNGLOBJECTPTRLABEL_PROC*)get_proc("glObjectPtrLabel");
    glpfPopDebugGroup = (PFNGLPOPDEBUGGROUP_PROC*)get_proc("glPopDebugGroup");
    glpfPushDebugGroup = (PFNGLPUSHDEBUGGROUP_PROC*)get_proc("glPushDebugGroup");
    glpfShaderStorageBlockBinding = (PFNGLSHADERSTORAGEBLOCKBINDING_PROC*)get_proc("glShaderStorageBlockBinding");
    glpfTexBufferRange = (PFNGLTEXBUFFERRANGE_PROC*)get_proc("glTexBufferRange");
    glpfTexStorage2DMultisample = (PFNGLTEXSTORAGE2DMULTISAMPLE_PROC*)get_proc("glTexStorage2DMultisample");
    glpfTexStorage3DMultisample = (PFNGLTEXSTORAGE3DMULTISAMPLE_PROC*)get_proc("glTexStorage3DMultisample");
    glpfTextureView = (PFNGLTEXTUREVIEW_PROC*)get_proc("glTextureView");
    glpfVertexAttribBinding = (PFNGLVERTEXATTRIBBINDING_PROC*)get_proc("glVertexAttribBinding");
    glpfVertexAttribFormat = (PFNGLVERTEXATTRIBFORMAT_PROC*)get_proc("glVertexAttribFormat");
    glpfVertexAttribIFormat = (PFNGLVERTEXATTRIBIFORMAT_PROC*)get_proc("glVertexAttribIFormat");
    glpfVertexAttribLFormat = (PFNGLVERTEXATTRIBLFORMAT_PROC*)get_proc("glVertexAttribLFormat");
    glpfVertexBindingDivisor = (PFNGLVERTEXBINDINGDIVISOR_PROC*)get_proc("glVertexBindingDivisor");


    /* GL_VERSION_4_4 */

    glpfBindBuffersBase = (PFNGLBINDBUFFERSBASE_PROC*)get_proc("glBindBuffersBase");
    glpfBindBuffersRange = (PFNGLBINDBUFFERSRANGE_PROC*)get_proc("glBindBuffersRange");
    glpfBindImageTextures = (PFNGLBINDIMAGETEXTURES_PROC*)get_proc("glBindImageTextures");
    glpfBindSamplers = (PFNGLBINDSAMPLERS_PROC*)get_proc("glBindSamplers");
    glpfBindTextures = (PFNGLBINDTEXTURES_PROC*)get_proc("glBindTextures");
    glpfBindVertexBuffers = (PFNGLBINDVERTEXBUFFERS_PROC*)get_proc("glBindVertexBuffers");
    glpfBufferStorage = (PFNGLBUFFERSTORAGE_PROC*)get_proc("glBufferStorage");
    glpfClearTexImage = (PFNGLCLEARTEXIMAGE_PROC*)get_proc("glClearTexImage");
    glpfClearTexSubImage = (PFNGLCLEARTEXSUBIMAGE_PROC*)get_proc("glClearTexSubImage");


}

/* ----------------------- Extension flag definitions ---------------------- */
int FLEXT_ARB_bindless_texture = GL_FALSE;
int FLEXT_EXT_direct_state_access = GL_FALSE;
int FLEXT_KHR_texture_compression_astc_ldr = GL_FALSE;
int FLEXT_KHR_texture_compression_astc_hdr = GL_FALSE;
int FLEXT_ARB_direct_state_access = GL_FALSE;

/* ---------------------- Function pointer definitions --------------------- */

/* GL_ARB_bindless_texture */

PFNGLGETIMAGEHANDLEARB_PROC* glpfGetImageHandleARB = NULL;
PFNGLGETTEXTUREHANDLEARB_PROC* glpfGetTextureHandleARB = NULL;
PFNGLGETTEXTURESAMPLERHANDLEARB_PROC* glpfGetTextureSamplerHandleARB = NULL;
PFNGLGETVERTEXATTRIBLUI64VARB_PROC* glpfGetVertexAttribLui64vARB = NULL;
PFNGLISIMAGEHANDLERESIDENTARB_PROC* glpfIsImageHandleResidentARB = NULL;
PFNGLISTEXTUREHANDLERESIDENTARB_PROC* glpfIsTextureHandleResidentARB = NULL;
PFNGLMAKEIMAGEHANDLENONRESIDENTARB_PROC* glpfMakeImageHandleNonResidentARB = NULL;
PFNGLMAKEIMAGEHANDLERESIDENTARB_PROC* glpfMakeImageHandleResidentARB = NULL;
PFNGLMAKETEXTUREHANDLENONRESIDENTARB_PROC* glpfMakeTextureHandleNonResidentARB = NULL;
PFNGLMAKETEXTUREHANDLERESIDENTARB_PROC* glpfMakeTextureHandleResidentARB = NULL;
PFNGLPROGRAMUNIFORMHANDLEUI64ARB_PROC* glpfProgramUniformHandleui64ARB = NULL;
PFNGLPROGRAMUNIFORMHANDLEUI64VARB_PROC* glpfProgramUniformHandleui64vARB = NULL;
PFNGLUNIFORMHANDLEUI64ARB_PROC* glpfUniformHandleui64ARB = NULL;
PFNGLUNIFORMHANDLEUI64VARB_PROC* glpfUniformHandleui64vARB = NULL;
PFNGLVERTEXATTRIBL1UI64ARB_PROC* glpfVertexAttribL1ui64ARB = NULL;
PFNGLVERTEXATTRIBL1UI64VARB_PROC* glpfVertexAttribL1ui64vARB = NULL;

/* GL_ARB_direct_state_access */

PFNGLBINDTEXTUREUNIT_PROC* glpfBindTextureUnit = NULL;
PFNGLBLITNAMEDFRAMEBUFFER_PROC* glpfBlitNamedFramebuffer = NULL;
PFNGLCHECKNAMEDFRAMEBUFFERSTATUS_PROC* glpfCheckNamedFramebufferStatus = NULL;
PFNGLCLEARNAMEDBUFFERDATA_PROC* glpfClearNamedBufferData = NULL;
PFNGLCLEARNAMEDBUFFERSUBDATA_PROC* glpfClearNamedBufferSubData = NULL;
PFNGLCLEARNAMEDFRAMEBUFFERFI_PROC* glpfClearNamedFramebufferfi = NULL;
PFNGLCLEARNAMEDFRAMEBUFFERFV_PROC* glpfClearNamedFramebufferfv = NULL;
PFNGLCLEARNAMEDFRAMEBUFFERIV_PROC* glpfClearNamedFramebufferiv = NULL;
PFNGLCLEARNAMEDFRAMEBUFFERUIV_PROC* glpfClearNamedFramebufferuiv = NULL;
PFNGLCOMPRESSEDTEXTURESUBIMAGE1D_PROC* glpfCompressedTextureSubImage1D = NULL;
PFNGLCOMPRESSEDTEXTURESUBIMAGE2D_PROC* glpfCompressedTextureSubImage2D = NULL;
PFNGLCOMPRESSEDTEXTURESUBIMAGE3D_PROC* glpfCompressedTextureSubImage3D = NULL;
PFNGLCOPYNAMEDBUFFERSUBDATA_PROC* glpfCopyNamedBufferSubData = NULL;
PFNGLCOPYTEXTURESUBIMAGE1D_PROC* glpfCopyTextureSubImage1D = NULL;
PFNGLCOPYTEXTURESUBIMAGE2D_PROC* glpfCopyTextureSubImage2D = NULL;
PFNGLCOPYTEXTURESUBIMAGE3D_PROC* glpfCopyTextureSubImage3D = NULL;
PFNGLCREATEBUFFERS_PROC* glpfCreateBuffers = NULL;
PFNGLCREATEFRAMEBUFFERS_PROC* glpfCreateFramebuffers = NULL;
PFNGLCREATEPROGRAMPIPELINES_PROC* glpfCreateProgramPipelines = NULL;
PFNGLCREATEQUERIES_PROC* glpfCreateQueries = NULL;
PFNGLCREATERENDERBUFFERS_PROC* glpfCreateRenderbuffers = NULL;
PFNGLCREATESAMPLERS_PROC* glpfCreateSamplers = NULL;
PFNGLCREATETEXTURES_PROC* glpfCreateTextures = NULL;
PFNGLCREATETRANSFORMFEEDBACKS_PROC* glpfCreateTransformFeedbacks = NULL;
PFNGLCREATEVERTEXARRAYS_PROC* glpfCreateVertexArrays = NULL;
PFNGLDISABLEVERTEXARRAYATTRIB_PROC* glpfDisableVertexArrayAttrib = NULL;
PFNGLENABLEVERTEXARRAYATTRIB_PROC* glpfEnableVertexArrayAttrib = NULL;
PFNGLFLUSHMAPPEDNAMEDBUFFERRANGE_PROC* glpfFlushMappedNamedBufferRange = NULL;
PFNGLGENERATETEXTUREMIPMAP_PROC* glpfGenerateTextureMipmap = NULL;
PFNGLGETCOMPRESSEDTEXTUREIMAGE_PROC* glpfGetCompressedTextureImage = NULL;
PFNGLGETNAMEDBUFFERPARAMETERI64V_PROC* glpfGetNamedBufferParameteri64v = NULL;
PFNGLGETNAMEDBUFFERPARAMETERIV_PROC* glpfGetNamedBufferParameteriv = NULL;
PFNGLGETNAMEDBUFFERPOINTERV_PROC* glpfGetNamedBufferPointerv = NULL;
PFNGLGETNAMEDBUFFERSUBDATA_PROC* glpfGetNamedBufferSubData = NULL;
PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIV_PROC* glpfGetNamedFramebufferAttachmentParameteriv = NULL;
PFNGLGETNAMEDFRAMEBUFFERPARAMETERIV_PROC* glpfGetNamedFramebufferParameteriv = NULL;
PFNGLGETNAMEDRENDERBUFFERPARAMETERIV_PROC* glpfGetNamedRenderbufferParameteriv = NULL;
PFNGLGETQUERYBUFFEROBJECTI64V_PROC* glpfGetQueryBufferObjecti64v = NULL;
PFNGLGETQUERYBUFFEROBJECTIV_PROC* glpfGetQueryBufferObjectiv = NULL;
PFNGLGETQUERYBUFFEROBJECTUI64V_PROC* glpfGetQueryBufferObjectui64v = NULL;
PFNGLGETQUERYBUFFEROBJECTUIV_PROC* glpfGetQueryBufferObjectuiv = NULL;
PFNGLGETTEXTUREIMAGE_PROC* glpfGetTextureImage = NULL;
PFNGLGETTEXTURELEVELPARAMETERFV_PROC* glpfGetTextureLevelParameterfv = NULL;
PFNGLGETTEXTURELEVELPARAMETERIV_PROC* glpfGetTextureLevelParameteriv = NULL;
PFNGLGETTEXTUREPARAMETERIIV_PROC* glpfGetTextureParameterIiv = NULL;
PFNGLGETTEXTUREPARAMETERIUIV_PROC* glpfGetTextureParameterIuiv = NULL;
PFNGLGETTEXTUREPARAMETERFV_PROC* glpfGetTextureParameterfv = NULL;
PFNGLGETTEXTUREPARAMETERIV_PROC* glpfGetTextureParameteriv = NULL;
PFNGLGETTRANSFORMFEEDBACKI64_V_PROC* glpfGetTransformFeedbacki64_v = NULL;
PFNGLGETTRANSFORMFEEDBACKI_V_PROC* glpfGetTransformFeedbacki_v = NULL;
PFNGLGETTRANSFORMFEEDBACKIV_PROC* glpfGetTransformFeedbackiv = NULL;
PFNGLGETVERTEXARRAYINDEXED64IV_PROC* glpfGetVertexArrayIndexed64iv = NULL;
PFNGLGETVERTEXARRAYINDEXEDIV_PROC* glpfGetVertexArrayIndexediv = NULL;
PFNGLGETVERTEXARRAYIV_PROC* glpfGetVertexArrayiv = NULL;
PFNGLINVALIDATENAMEDFRAMEBUFFERDATA_PROC* glpfInvalidateNamedFramebufferData = NULL;
PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATA_PROC* glpfInvalidateNamedFramebufferSubData = NULL;
PFNGLMAPNAMEDBUFFER_PROC* glpfMapNamedBuffer = NULL;
PFNGLMAPNAMEDBUFFERRANGE_PROC* glpfMapNamedBufferRange = NULL;
PFNGLNAMEDBUFFERDATA_PROC* glpfNamedBufferData = NULL;
PFNGLNAMEDBUFFERSTORAGE_PROC* glpfNamedBufferStorage = NULL;
PFNGLNAMEDBUFFERSUBDATA_PROC* glpfNamedBufferSubData = NULL;
PFNGLNAMEDFRAMEBUFFERDRAWBUFFER_PROC* glpfNamedFramebufferDrawBuffer = NULL;
PFNGLNAMEDFRAMEBUFFERDRAWBUFFERS_PROC* glpfNamedFramebufferDrawBuffers = NULL;
PFNGLNAMEDFRAMEBUFFERPARAMETERI_PROC* glpfNamedFramebufferParameteri = NULL;
PFNGLNAMEDFRAMEBUFFERREADBUFFER_PROC* glpfNamedFramebufferReadBuffer = NULL;
PFNGLNAMEDFRAMEBUFFERRENDERBUFFER_PROC* glpfNamedFramebufferRenderbuffer = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTURE_PROC* glpfNamedFramebufferTexture = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTURELAYER_PROC* glpfNamedFramebufferTextureLayer = NULL;
PFNGLNAMEDRENDERBUFFERSTORAGE_PROC* glpfNamedRenderbufferStorage = NULL;
PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLE_PROC* glpfNamedRenderbufferStorageMultisample = NULL;
PFNGLTEXTUREBUFFER_PROC* glpfTextureBuffer = NULL;
PFNGLTEXTUREBUFFERRANGE_PROC* glpfTextureBufferRange = NULL;
PFNGLTEXTUREPARAMETERIIV_PROC* glpfTextureParameterIiv = NULL;
PFNGLTEXTUREPARAMETERIUIV_PROC* glpfTextureParameterIuiv = NULL;
PFNGLTEXTUREPARAMETERF_PROC* glpfTextureParameterf = NULL;
PFNGLTEXTUREPARAMETERFV_PROC* glpfTextureParameterfv = NULL;
PFNGLTEXTUREPARAMETERI_PROC* glpfTextureParameteri = NULL;
PFNGLTEXTUREPARAMETERIV_PROC* glpfTextureParameteriv = NULL;
PFNGLTEXTURESTORAGE1D_PROC* glpfTextureStorage1D = NULL;
PFNGLTEXTURESTORAGE2D_PROC* glpfTextureStorage2D = NULL;
PFNGLTEXTURESTORAGE2DMULTISAMPLE_PROC* glpfTextureStorage2DMultisample = NULL;
PFNGLTEXTURESTORAGE3D_PROC* glpfTextureStorage3D = NULL;
PFNGLTEXTURESTORAGE3DMULTISAMPLE_PROC* glpfTextureStorage3DMultisample = NULL;
PFNGLTEXTURESUBIMAGE1D_PROC* glpfTextureSubImage1D = NULL;
PFNGLTEXTURESUBIMAGE2D_PROC* glpfTextureSubImage2D = NULL;
PFNGLTEXTURESUBIMAGE3D_PROC* glpfTextureSubImage3D = NULL;
PFNGLTRANSFORMFEEDBACKBUFFERBASE_PROC* glpfTransformFeedbackBufferBase = NULL;
PFNGLTRANSFORMFEEDBACKBUFFERRANGE_PROC* glpfTransformFeedbackBufferRange = NULL;
PFNGLUNMAPNAMEDBUFFER_PROC* glpfUnmapNamedBuffer = NULL;
PFNGLVERTEXARRAYATTRIBBINDING_PROC* glpfVertexArrayAttribBinding = NULL;
PFNGLVERTEXARRAYATTRIBFORMAT_PROC* glpfVertexArrayAttribFormat = NULL;
PFNGLVERTEXARRAYATTRIBIFORMAT_PROC* glpfVertexArrayAttribIFormat = NULL;
PFNGLVERTEXARRAYATTRIBLFORMAT_PROC* glpfVertexArrayAttribLFormat = NULL;
PFNGLVERTEXARRAYBINDINGDIVISOR_PROC* glpfVertexArrayBindingDivisor = NULL;
PFNGLVERTEXARRAYELEMENTBUFFER_PROC* glpfVertexArrayElementBuffer = NULL;
PFNGLVERTEXARRAYVERTEXBUFFER_PROC* glpfVertexArrayVertexBuffer = NULL;
PFNGLVERTEXARRAYVERTEXBUFFERS_PROC* glpfVertexArrayVertexBuffers = NULL;

/* GL_EXT_direct_state_access */

PFNGLBINDMULTITEXTUREEXT_PROC* glpfBindMultiTextureEXT = NULL;
PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXT_PROC* glpfCheckNamedFramebufferStatusEXT = NULL;
PFNGLCLEARNAMEDBUFFERDATAEXT_PROC* glpfClearNamedBufferDataEXT = NULL;
PFNGLCLEARNAMEDBUFFERSUBDATAEXT_PROC* glpfClearNamedBufferSubDataEXT = NULL;
PFNGLCLIENTATTRIBDEFAULTEXT_PROC* glpfClientAttribDefaultEXT = NULL;
PFNGLCOMPRESSEDMULTITEXIMAGE1DEXT_PROC* glpfCompressedMultiTexImage1DEXT = NULL;
PFNGLCOMPRESSEDMULTITEXIMAGE2DEXT_PROC* glpfCompressedMultiTexImage2DEXT = NULL;
PFNGLCOMPRESSEDMULTITEXIMAGE3DEXT_PROC* glpfCompressedMultiTexImage3DEXT = NULL;
PFNGLCOMPRESSEDMULTITEXSUBIMAGE1DEXT_PROC* glpfCompressedMultiTexSubImage1DEXT = NULL;
PFNGLCOMPRESSEDMULTITEXSUBIMAGE2DEXT_PROC* glpfCompressedMultiTexSubImage2DEXT = NULL;
PFNGLCOMPRESSEDMULTITEXSUBIMAGE3DEXT_PROC* glpfCompressedMultiTexSubImage3DEXT = NULL;
PFNGLCOMPRESSEDTEXTUREIMAGE1DEXT_PROC* glpfCompressedTextureImage1DEXT = NULL;
PFNGLCOMPRESSEDTEXTUREIMAGE2DEXT_PROC* glpfCompressedTextureImage2DEXT = NULL;
PFNGLCOMPRESSEDTEXTUREIMAGE3DEXT_PROC* glpfCompressedTextureImage3DEXT = NULL;
PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXT_PROC* glpfCompressedTextureSubImage1DEXT = NULL;
PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXT_PROC* glpfCompressedTextureSubImage2DEXT = NULL;
PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXT_PROC* glpfCompressedTextureSubImage3DEXT = NULL;
PFNGLCOPYMULTITEXIMAGE1DEXT_PROC* glpfCopyMultiTexImage1DEXT = NULL;
PFNGLCOPYMULTITEXIMAGE2DEXT_PROC* glpfCopyMultiTexImage2DEXT = NULL;
PFNGLCOPYMULTITEXSUBIMAGE1DEXT_PROC* glpfCopyMultiTexSubImage1DEXT = NULL;
PFNGLCOPYMULTITEXSUBIMAGE2DEXT_PROC* glpfCopyMultiTexSubImage2DEXT = NULL;
PFNGLCOPYMULTITEXSUBIMAGE3DEXT_PROC* glpfCopyMultiTexSubImage3DEXT = NULL;
PFNGLCOPYTEXTUREIMAGE1DEXT_PROC* glpfCopyTextureImage1DEXT = NULL;
PFNGLCOPYTEXTUREIMAGE2DEXT_PROC* glpfCopyTextureImage2DEXT = NULL;
PFNGLCOPYTEXTURESUBIMAGE1DEXT_PROC* glpfCopyTextureSubImage1DEXT = NULL;
PFNGLCOPYTEXTURESUBIMAGE2DEXT_PROC* glpfCopyTextureSubImage2DEXT = NULL;
PFNGLCOPYTEXTURESUBIMAGE3DEXT_PROC* glpfCopyTextureSubImage3DEXT = NULL;
PFNGLDISABLECLIENTSTATEINDEXEDEXT_PROC* glpfDisableClientStateIndexedEXT = NULL;
PFNGLDISABLECLIENTSTATEIEXT_PROC* glpfDisableClientStateiEXT = NULL;
PFNGLDISABLEINDEXEDEXT_PROC* glpfDisableIndexedEXT = NULL;
PFNGLDISABLEVERTEXARRAYATTRIBEXT_PROC* glpfDisableVertexArrayAttribEXT = NULL;
PFNGLDISABLEVERTEXARRAYEXT_PROC* glpfDisableVertexArrayEXT = NULL;
PFNGLENABLECLIENTSTATEINDEXEDEXT_PROC* glpfEnableClientStateIndexedEXT = NULL;
PFNGLENABLECLIENTSTATEIEXT_PROC* glpfEnableClientStateiEXT = NULL;
PFNGLENABLEINDEXEDEXT_PROC* glpfEnableIndexedEXT = NULL;
PFNGLENABLEVERTEXARRAYATTRIBEXT_PROC* glpfEnableVertexArrayAttribEXT = NULL;
PFNGLENABLEVERTEXARRAYEXT_PROC* glpfEnableVertexArrayEXT = NULL;
PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEEXT_PROC* glpfFlushMappedNamedBufferRangeEXT = NULL;
PFNGLFRAMEBUFFERDRAWBUFFEREXT_PROC* glpfFramebufferDrawBufferEXT = NULL;
PFNGLFRAMEBUFFERDRAWBUFFERSEXT_PROC* glpfFramebufferDrawBuffersEXT = NULL;
PFNGLFRAMEBUFFERREADBUFFEREXT_PROC* glpfFramebufferReadBufferEXT = NULL;
PFNGLGENERATEMULTITEXMIPMAPEXT_PROC* glpfGenerateMultiTexMipmapEXT = NULL;
PFNGLGENERATETEXTUREMIPMAPEXT_PROC* glpfGenerateTextureMipmapEXT = NULL;
PFNGLGETBOOLEANINDEXEDVEXT_PROC* glpfGetBooleanIndexedvEXT = NULL;
PFNGLGETCOMPRESSEDMULTITEXIMAGEEXT_PROC* glpfGetCompressedMultiTexImageEXT = NULL;
PFNGLGETCOMPRESSEDTEXTUREIMAGEEXT_PROC* glpfGetCompressedTextureImageEXT = NULL;
PFNGLGETDOUBLEINDEXEDVEXT_PROC* glpfGetDoubleIndexedvEXT = NULL;
PFNGLGETDOUBLEI_VEXT_PROC* glpfGetDoublei_vEXT = NULL;
PFNGLGETFLOATINDEXEDVEXT_PROC* glpfGetFloatIndexedvEXT = NULL;
PFNGLGETFLOATI_VEXT_PROC* glpfGetFloati_vEXT = NULL;
PFNGLGETFRAMEBUFFERPARAMETERIVEXT_PROC* glpfGetFramebufferParameterivEXT = NULL;
PFNGLGETINTEGERINDEXEDVEXT_PROC* glpfGetIntegerIndexedvEXT = NULL;
PFNGLGETMULTITEXENVFVEXT_PROC* glpfGetMultiTexEnvfvEXT = NULL;
PFNGLGETMULTITEXENVIVEXT_PROC* glpfGetMultiTexEnvivEXT = NULL;
PFNGLGETMULTITEXGENDVEXT_PROC* glpfGetMultiTexGendvEXT = NULL;
PFNGLGETMULTITEXGENFVEXT_PROC* glpfGetMultiTexGenfvEXT = NULL;
PFNGLGETMULTITEXGENIVEXT_PROC* glpfGetMultiTexGenivEXT = NULL;
PFNGLGETMULTITEXIMAGEEXT_PROC* glpfGetMultiTexImageEXT = NULL;
PFNGLGETMULTITEXLEVELPARAMETERFVEXT_PROC* glpfGetMultiTexLevelParameterfvEXT = NULL;
PFNGLGETMULTITEXLEVELPARAMETERIVEXT_PROC* glpfGetMultiTexLevelParameterivEXT = NULL;
PFNGLGETMULTITEXPARAMETERIIVEXT_PROC* glpfGetMultiTexParameterIivEXT = NULL;
PFNGLGETMULTITEXPARAMETERIUIVEXT_PROC* glpfGetMultiTexParameterIuivEXT = NULL;
PFNGLGETMULTITEXPARAMETERFVEXT_PROC* glpfGetMultiTexParameterfvEXT = NULL;
PFNGLGETMULTITEXPARAMETERIVEXT_PROC* glpfGetMultiTexParameterivEXT = NULL;
PFNGLGETNAMEDBUFFERPARAMETERIVEXT_PROC* glpfGetNamedBufferParameterivEXT = NULL;
PFNGLGETNAMEDBUFFERPOINTERVEXT_PROC* glpfGetNamedBufferPointervEXT = NULL;
PFNGLGETNAMEDBUFFERSUBDATAEXT_PROC* glpfGetNamedBufferSubDataEXT = NULL;
PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXT_PROC* glpfGetNamedFramebufferAttachmentParameterivEXT = NULL;
PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVEXT_PROC* glpfGetNamedFramebufferParameterivEXT = NULL;
PFNGLGETNAMEDPROGRAMLOCALPARAMETERIIVEXT_PROC* glpfGetNamedProgramLocalParameterIivEXT = NULL;
PFNGLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXT_PROC* glpfGetNamedProgramLocalParameterIuivEXT = NULL;
PFNGLGETNAMEDPROGRAMLOCALPARAMETERDVEXT_PROC* glpfGetNamedProgramLocalParameterdvEXT = NULL;
PFNGLGETNAMEDPROGRAMLOCALPARAMETERFVEXT_PROC* glpfGetNamedProgramLocalParameterfvEXT = NULL;
PFNGLGETNAMEDPROGRAMSTRINGEXT_PROC* glpfGetNamedProgramStringEXT = NULL;
PFNGLGETNAMEDPROGRAMIVEXT_PROC* glpfGetNamedProgramivEXT = NULL;
PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXT_PROC* glpfGetNamedRenderbufferParameterivEXT = NULL;
PFNGLGETPOINTERINDEXEDVEXT_PROC* glpfGetPointerIndexedvEXT = NULL;
PFNGLGETPOINTERI_VEXT_PROC* glpfGetPointeri_vEXT = NULL;
PFNGLGETTEXTUREIMAGEEXT_PROC* glpfGetTextureImageEXT = NULL;
PFNGLGETTEXTURELEVELPARAMETERFVEXT_PROC* glpfGetTextureLevelParameterfvEXT = NULL;
PFNGLGETTEXTURELEVELPARAMETERIVEXT_PROC* glpfGetTextureLevelParameterivEXT = NULL;
PFNGLGETTEXTUREPARAMETERIIVEXT_PROC* glpfGetTextureParameterIivEXT = NULL;
PFNGLGETTEXTUREPARAMETERIUIVEXT_PROC* glpfGetTextureParameterIuivEXT = NULL;
PFNGLGETTEXTUREPARAMETERFVEXT_PROC* glpfGetTextureParameterfvEXT = NULL;
PFNGLGETTEXTUREPARAMETERIVEXT_PROC* glpfGetTextureParameterivEXT = NULL;
PFNGLGETVERTEXARRAYINTEGERI_VEXT_PROC* glpfGetVertexArrayIntegeri_vEXT = NULL;
PFNGLGETVERTEXARRAYINTEGERVEXT_PROC* glpfGetVertexArrayIntegervEXT = NULL;
PFNGLGETVERTEXARRAYPOINTERI_VEXT_PROC* glpfGetVertexArrayPointeri_vEXT = NULL;
PFNGLGETVERTEXARRAYPOINTERVEXT_PROC* glpfGetVertexArrayPointervEXT = NULL;
PFNGLISENABLEDINDEXEDEXT_PROC* glpfIsEnabledIndexedEXT = NULL;
PFNGLMAPNAMEDBUFFEREXT_PROC* glpfMapNamedBufferEXT = NULL;
PFNGLMAPNAMEDBUFFERRANGEEXT_PROC* glpfMapNamedBufferRangeEXT = NULL;
PFNGLMATRIXFRUSTUMEXT_PROC* glpfMatrixFrustumEXT = NULL;
PFNGLMATRIXLOADIDENTITYEXT_PROC* glpfMatrixLoadIdentityEXT = NULL;
PFNGLMATRIXLOADTRANSPOSEDEXT_PROC* glpfMatrixLoadTransposedEXT = NULL;
PFNGLMATRIXLOADTRANSPOSEFEXT_PROC* glpfMatrixLoadTransposefEXT = NULL;
PFNGLMATRIXLOADDEXT_PROC* glpfMatrixLoaddEXT = NULL;
PFNGLMATRIXLOADFEXT_PROC* glpfMatrixLoadfEXT = NULL;
PFNGLMATRIXMULTTRANSPOSEDEXT_PROC* glpfMatrixMultTransposedEXT = NULL;
PFNGLMATRIXMULTTRANSPOSEFEXT_PROC* glpfMatrixMultTransposefEXT = NULL;
PFNGLMATRIXMULTDEXT_PROC* glpfMatrixMultdEXT = NULL;
PFNGLMATRIXMULTFEXT_PROC* glpfMatrixMultfEXT = NULL;
PFNGLMATRIXORTHOEXT_PROC* glpfMatrixOrthoEXT = NULL;
PFNGLMATRIXPOPEXT_PROC* glpfMatrixPopEXT = NULL;
PFNGLMATRIXPUSHEXT_PROC* glpfMatrixPushEXT = NULL;
PFNGLMATRIXROTATEDEXT_PROC* glpfMatrixRotatedEXT = NULL;
PFNGLMATRIXROTATEFEXT_PROC* glpfMatrixRotatefEXT = NULL;
PFNGLMATRIXSCALEDEXT_PROC* glpfMatrixScaledEXT = NULL;
PFNGLMATRIXSCALEFEXT_PROC* glpfMatrixScalefEXT = NULL;
PFNGLMATRIXTRANSLATEDEXT_PROC* glpfMatrixTranslatedEXT = NULL;
PFNGLMATRIXTRANSLATEFEXT_PROC* glpfMatrixTranslatefEXT = NULL;
PFNGLMULTITEXBUFFEREXT_PROC* glpfMultiTexBufferEXT = NULL;
PFNGLMULTITEXCOORDPOINTEREXT_PROC* glpfMultiTexCoordPointerEXT = NULL;
PFNGLMULTITEXENVFEXT_PROC* glpfMultiTexEnvfEXT = NULL;
PFNGLMULTITEXENVFVEXT_PROC* glpfMultiTexEnvfvEXT = NULL;
PFNGLMULTITEXENVIEXT_PROC* glpfMultiTexEnviEXT = NULL;
PFNGLMULTITEXENVIVEXT_PROC* glpfMultiTexEnvivEXT = NULL;
PFNGLMULTITEXGENDEXT_PROC* glpfMultiTexGendEXT = NULL;
PFNGLMULTITEXGENDVEXT_PROC* glpfMultiTexGendvEXT = NULL;
PFNGLMULTITEXGENFEXT_PROC* glpfMultiTexGenfEXT = NULL;
PFNGLMULTITEXGENFVEXT_PROC* glpfMultiTexGenfvEXT = NULL;
PFNGLMULTITEXGENIEXT_PROC* glpfMultiTexGeniEXT = NULL;
PFNGLMULTITEXGENIVEXT_PROC* glpfMultiTexGenivEXT = NULL;
PFNGLMULTITEXIMAGE1DEXT_PROC* glpfMultiTexImage1DEXT = NULL;
PFNGLMULTITEXIMAGE2DEXT_PROC* glpfMultiTexImage2DEXT = NULL;
PFNGLMULTITEXIMAGE3DEXT_PROC* glpfMultiTexImage3DEXT = NULL;
PFNGLMULTITEXPARAMETERIIVEXT_PROC* glpfMultiTexParameterIivEXT = NULL;
PFNGLMULTITEXPARAMETERIUIVEXT_PROC* glpfMultiTexParameterIuivEXT = NULL;
PFNGLMULTITEXPARAMETERFEXT_PROC* glpfMultiTexParameterfEXT = NULL;
PFNGLMULTITEXPARAMETERFVEXT_PROC* glpfMultiTexParameterfvEXT = NULL;
PFNGLMULTITEXPARAMETERIEXT_PROC* glpfMultiTexParameteriEXT = NULL;
PFNGLMULTITEXPARAMETERIVEXT_PROC* glpfMultiTexParameterivEXT = NULL;
PFNGLMULTITEXRENDERBUFFEREXT_PROC* glpfMultiTexRenderbufferEXT = NULL;
PFNGLMULTITEXSUBIMAGE1DEXT_PROC* glpfMultiTexSubImage1DEXT = NULL;
PFNGLMULTITEXSUBIMAGE2DEXT_PROC* glpfMultiTexSubImage2DEXT = NULL;
PFNGLMULTITEXSUBIMAGE3DEXT_PROC* glpfMultiTexSubImage3DEXT = NULL;
PFNGLNAMEDBUFFERDATAEXT_PROC* glpfNamedBufferDataEXT = NULL;
PFNGLNAMEDBUFFERSTORAGEEXT_PROC* glpfNamedBufferStorageEXT = NULL;
PFNGLNAMEDBUFFERSUBDATAEXT_PROC* glpfNamedBufferSubDataEXT = NULL;
PFNGLNAMEDCOPYBUFFERSUBDATAEXT_PROC* glpfNamedCopyBufferSubDataEXT = NULL;
PFNGLNAMEDFRAMEBUFFERPARAMETERIEXT_PROC* glpfNamedFramebufferParameteriEXT = NULL;
PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXT_PROC* glpfNamedFramebufferRenderbufferEXT = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXT_PROC* glpfNamedFramebufferTexture1DEXT = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXT_PROC* glpfNamedFramebufferTexture2DEXT = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXT_PROC* glpfNamedFramebufferTexture3DEXT = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTUREEXT_PROC* glpfNamedFramebufferTextureEXT = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXT_PROC* glpfNamedFramebufferTextureFaceEXT = NULL;
PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXT_PROC* glpfNamedFramebufferTextureLayerEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETER4DEXT_PROC* glpfNamedProgramLocalParameter4dEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETER4DVEXT_PROC* glpfNamedProgramLocalParameter4dvEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETER4FEXT_PROC* glpfNamedProgramLocalParameter4fEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETER4FVEXT_PROC* glpfNamedProgramLocalParameter4fvEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERI4IEXT_PROC* glpfNamedProgramLocalParameterI4iEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERI4IVEXT_PROC* glpfNamedProgramLocalParameterI4ivEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIEXT_PROC* glpfNamedProgramLocalParameterI4uiEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIVEXT_PROC* glpfNamedProgramLocalParameterI4uivEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERS4FVEXT_PROC* glpfNamedProgramLocalParameters4fvEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERSI4IVEXT_PROC* glpfNamedProgramLocalParametersI4ivEXT = NULL;
PFNGLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXT_PROC* glpfNamedProgramLocalParametersI4uivEXT = NULL;
PFNGLNAMEDPROGRAMSTRINGEXT_PROC* glpfNamedProgramStringEXT = NULL;
PFNGLNAMEDRENDERBUFFERSTORAGEEXT_PROC* glpfNamedRenderbufferStorageEXT = NULL;
PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXT_PROC* glpfNamedRenderbufferStorageMultisampleCoverageEXT = NULL;
PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXT_PROC* glpfNamedRenderbufferStorageMultisampleEXT = NULL;
PFNGLPROGRAMUNIFORM1DEXT_PROC* glpfProgramUniform1dEXT = NULL;
PFNGLPROGRAMUNIFORM1DVEXT_PROC* glpfProgramUniform1dvEXT = NULL;
PFNGLPROGRAMUNIFORM1FEXT_PROC* glpfProgramUniform1fEXT = NULL;
PFNGLPROGRAMUNIFORM1FVEXT_PROC* glpfProgramUniform1fvEXT = NULL;
PFNGLPROGRAMUNIFORM1IEXT_PROC* glpfProgramUniform1iEXT = NULL;
PFNGLPROGRAMUNIFORM1IVEXT_PROC* glpfProgramUniform1ivEXT = NULL;
PFNGLPROGRAMUNIFORM1UIEXT_PROC* glpfProgramUniform1uiEXT = NULL;
PFNGLPROGRAMUNIFORM1UIVEXT_PROC* glpfProgramUniform1uivEXT = NULL;
PFNGLPROGRAMUNIFORM2DEXT_PROC* glpfProgramUniform2dEXT = NULL;
PFNGLPROGRAMUNIFORM2DVEXT_PROC* glpfProgramUniform2dvEXT = NULL;
PFNGLPROGRAMUNIFORM2FEXT_PROC* glpfProgramUniform2fEXT = NULL;
PFNGLPROGRAMUNIFORM2FVEXT_PROC* glpfProgramUniform2fvEXT = NULL;
PFNGLPROGRAMUNIFORM2IEXT_PROC* glpfProgramUniform2iEXT = NULL;
PFNGLPROGRAMUNIFORM2IVEXT_PROC* glpfProgramUniform2ivEXT = NULL;
PFNGLPROGRAMUNIFORM2UIEXT_PROC* glpfProgramUniform2uiEXT = NULL;
PFNGLPROGRAMUNIFORM2UIVEXT_PROC* glpfProgramUniform2uivEXT = NULL;
PFNGLPROGRAMUNIFORM3DEXT_PROC* glpfProgramUniform3dEXT = NULL;
PFNGLPROGRAMUNIFORM3DVEXT_PROC* glpfProgramUniform3dvEXT = NULL;
PFNGLPROGRAMUNIFORM3FEXT_PROC* glpfProgramUniform3fEXT = NULL;
PFNGLPROGRAMUNIFORM3FVEXT_PROC* glpfProgramUniform3fvEXT = NULL;
PFNGLPROGRAMUNIFORM3IEXT_PROC* glpfProgramUniform3iEXT = NULL;
PFNGLPROGRAMUNIFORM3IVEXT_PROC* glpfProgramUniform3ivEXT = NULL;
PFNGLPROGRAMUNIFORM3UIEXT_PROC* glpfProgramUniform3uiEXT = NULL;
PFNGLPROGRAMUNIFORM3UIVEXT_PROC* glpfProgramUniform3uivEXT = NULL;
PFNGLPROGRAMUNIFORM4DEXT_PROC* glpfProgramUniform4dEXT = NULL;
PFNGLPROGRAMUNIFORM4DVEXT_PROC* glpfProgramUniform4dvEXT = NULL;
PFNGLPROGRAMUNIFORM4FEXT_PROC* glpfProgramUniform4fEXT = NULL;
PFNGLPROGRAMUNIFORM4FVEXT_PROC* glpfProgramUniform4fvEXT = NULL;
PFNGLPROGRAMUNIFORM4IEXT_PROC* glpfProgramUniform4iEXT = NULL;
PFNGLPROGRAMUNIFORM4IVEXT_PROC* glpfProgramUniform4ivEXT = NULL;
PFNGLPROGRAMUNIFORM4UIEXT_PROC* glpfProgramUniform4uiEXT = NULL;
PFNGLPROGRAMUNIFORM4UIVEXT_PROC* glpfProgramUniform4uivEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX2DVEXT_PROC* glpfProgramUniformMatrix2dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX2FVEXT_PROC* glpfProgramUniformMatrix2fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X3DVEXT_PROC* glpfProgramUniformMatrix2x3dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X3FVEXT_PROC* glpfProgramUniformMatrix2x3fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X4DVEXT_PROC* glpfProgramUniformMatrix2x4dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X4FVEXT_PROC* glpfProgramUniformMatrix2x4fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX3DVEXT_PROC* glpfProgramUniformMatrix3dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX3FVEXT_PROC* glpfProgramUniformMatrix3fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X2DVEXT_PROC* glpfProgramUniformMatrix3x2dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X2FVEXT_PROC* glpfProgramUniformMatrix3x2fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X4DVEXT_PROC* glpfProgramUniformMatrix3x4dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X4FVEXT_PROC* glpfProgramUniformMatrix3x4fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX4DVEXT_PROC* glpfProgramUniformMatrix4dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX4FVEXT_PROC* glpfProgramUniformMatrix4fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X2DVEXT_PROC* glpfProgramUniformMatrix4x2dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X2FVEXT_PROC* glpfProgramUniformMatrix4x2fvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X3DVEXT_PROC* glpfProgramUniformMatrix4x3dvEXT = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X3FVEXT_PROC* glpfProgramUniformMatrix4x3fvEXT = NULL;
PFNGLPUSHCLIENTATTRIBDEFAULTEXT_PROC* glpfPushClientAttribDefaultEXT = NULL;
PFNGLTEXTUREBUFFEREXT_PROC* glpfTextureBufferEXT = NULL;
PFNGLTEXTUREBUFFERRANGEEXT_PROC* glpfTextureBufferRangeEXT = NULL;
PFNGLTEXTUREIMAGE1DEXT_PROC* glpfTextureImage1DEXT = NULL;
PFNGLTEXTUREIMAGE2DEXT_PROC* glpfTextureImage2DEXT = NULL;
PFNGLTEXTUREIMAGE3DEXT_PROC* glpfTextureImage3DEXT = NULL;
PFNGLTEXTUREPAGECOMMITMENTEXT_PROC* glpfTexturePageCommitmentEXT = NULL;
PFNGLTEXTUREPARAMETERIIVEXT_PROC* glpfTextureParameterIivEXT = NULL;
PFNGLTEXTUREPARAMETERIUIVEXT_PROC* glpfTextureParameterIuivEXT = NULL;
PFNGLTEXTUREPARAMETERFEXT_PROC* glpfTextureParameterfEXT = NULL;
PFNGLTEXTUREPARAMETERFVEXT_PROC* glpfTextureParameterfvEXT = NULL;
PFNGLTEXTUREPARAMETERIEXT_PROC* glpfTextureParameteriEXT = NULL;
PFNGLTEXTUREPARAMETERIVEXT_PROC* glpfTextureParameterivEXT = NULL;
PFNGLTEXTURERENDERBUFFEREXT_PROC* glpfTextureRenderbufferEXT = NULL;
PFNGLTEXTURESTORAGE1DEXT_PROC* glpfTextureStorage1DEXT = NULL;
PFNGLTEXTURESTORAGE2DEXT_PROC* glpfTextureStorage2DEXT = NULL;
PFNGLTEXTURESTORAGE2DMULTISAMPLEEXT_PROC* glpfTextureStorage2DMultisampleEXT = NULL;
PFNGLTEXTURESTORAGE3DEXT_PROC* glpfTextureStorage3DEXT = NULL;
PFNGLTEXTURESTORAGE3DMULTISAMPLEEXT_PROC* glpfTextureStorage3DMultisampleEXT = NULL;
PFNGLTEXTURESUBIMAGE1DEXT_PROC* glpfTextureSubImage1DEXT = NULL;
PFNGLTEXTURESUBIMAGE2DEXT_PROC* glpfTextureSubImage2DEXT = NULL;
PFNGLTEXTURESUBIMAGE3DEXT_PROC* glpfTextureSubImage3DEXT = NULL;
PFNGLUNMAPNAMEDBUFFEREXT_PROC* glpfUnmapNamedBufferEXT = NULL;
PFNGLVERTEXARRAYBINDVERTEXBUFFEREXT_PROC* glpfVertexArrayBindVertexBufferEXT = NULL;
PFNGLVERTEXARRAYCOLOROFFSETEXT_PROC* glpfVertexArrayColorOffsetEXT = NULL;
PFNGLVERTEXARRAYEDGEFLAGOFFSETEXT_PROC* glpfVertexArrayEdgeFlagOffsetEXT = NULL;
PFNGLVERTEXARRAYFOGCOORDOFFSETEXT_PROC* glpfVertexArrayFogCoordOffsetEXT = NULL;
PFNGLVERTEXARRAYINDEXOFFSETEXT_PROC* glpfVertexArrayIndexOffsetEXT = NULL;
PFNGLVERTEXARRAYMULTITEXCOORDOFFSETEXT_PROC* glpfVertexArrayMultiTexCoordOffsetEXT = NULL;
PFNGLVERTEXARRAYNORMALOFFSETEXT_PROC* glpfVertexArrayNormalOffsetEXT = NULL;
PFNGLVERTEXARRAYSECONDARYCOLOROFFSETEXT_PROC* glpfVertexArraySecondaryColorOffsetEXT = NULL;
PFNGLVERTEXARRAYTEXCOORDOFFSETEXT_PROC* glpfVertexArrayTexCoordOffsetEXT = NULL;
PFNGLVERTEXARRAYVERTEXATTRIBBINDINGEXT_PROC* glpfVertexArrayVertexAttribBindingEXT = NULL;
PFNGLVERTEXARRAYVERTEXATTRIBDIVISOREXT_PROC* glpfVertexArrayVertexAttribDivisorEXT = NULL;
PFNGLVERTEXARRAYVERTEXATTRIBFORMATEXT_PROC* glpfVertexArrayVertexAttribFormatEXT = NULL;
PFNGLVERTEXARRAYVERTEXATTRIBIFORMATEXT_PROC* glpfVertexArrayVertexAttribIFormatEXT = NULL;
PFNGLVERTEXARRAYVERTEXATTRIBIOFFSETEXT_PROC* glpfVertexArrayVertexAttribIOffsetEXT = NULL;
PFNGLVERTEXARRAYVERTEXATTRIBLFORMATEXT_PROC* glpfVertexArrayVertexAttribLFormatEXT = NULL;
PFNGLVERTEXARRAYVERTEXATTRIBLOFFSETEXT_PROC* glpfVertexArrayVertexAttribLOffsetEXT = NULL;
PFNGLVERTEXARRAYVERTEXATTRIBOFFSETEXT_PROC* glpfVertexArrayVertexAttribOffsetEXT = NULL;
PFNGLVERTEXARRAYVERTEXBINDINGDIVISOREXT_PROC* glpfVertexArrayVertexBindingDivisorEXT = NULL;
PFNGLVERTEXARRAYVERTEXOFFSETEXT_PROC* glpfVertexArrayVertexOffsetEXT = NULL;

/* GL_VERSION_1_2 */

PFNGLCOPYTEXSUBIMAGE3D_PROC* glpfCopyTexSubImage3D = NULL;
PFNGLDRAWRANGEELEMENTS_PROC* glpfDrawRangeElements = NULL;
PFNGLTEXIMAGE3D_PROC* glpfTexImage3D = NULL;
PFNGLTEXSUBIMAGE3D_PROC* glpfTexSubImage3D = NULL;

/* GL_VERSION_1_3 */

PFNGLACTIVETEXTURE_PROC* glpfActiveTexture = NULL;
PFNGLCOMPRESSEDTEXIMAGE1D_PROC* glpfCompressedTexImage1D = NULL;
PFNGLCOMPRESSEDTEXIMAGE2D_PROC* glpfCompressedTexImage2D = NULL;
PFNGLCOMPRESSEDTEXIMAGE3D_PROC* glpfCompressedTexImage3D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE1D_PROC* glpfCompressedTexSubImage1D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE2D_PROC* glpfCompressedTexSubImage2D = NULL;
PFNGLCOMPRESSEDTEXSUBIMAGE3D_PROC* glpfCompressedTexSubImage3D = NULL;
PFNGLGETCOMPRESSEDTEXIMAGE_PROC* glpfGetCompressedTexImage = NULL;
PFNGLSAMPLECOVERAGE_PROC* glpfSampleCoverage = NULL;

/* GL_VERSION_1_4 */

PFNGLBLENDCOLOR_PROC* glpfBlendColor = NULL;
PFNGLBLENDEQUATION_PROC* glpfBlendEquation = NULL;
PFNGLBLENDFUNCSEPARATE_PROC* glpfBlendFuncSeparate = NULL;
PFNGLMULTIDRAWARRAYS_PROC* glpfMultiDrawArrays = NULL;
PFNGLMULTIDRAWELEMENTS_PROC* glpfMultiDrawElements = NULL;
PFNGLPOINTPARAMETERF_PROC* glpfPointParameterf = NULL;
PFNGLPOINTPARAMETERFV_PROC* glpfPointParameterfv = NULL;
PFNGLPOINTPARAMETERI_PROC* glpfPointParameteri = NULL;
PFNGLPOINTPARAMETERIV_PROC* glpfPointParameteriv = NULL;

/* GL_VERSION_1_5 */

PFNGLBEGINQUERY_PROC* glpfBeginQuery = NULL;
PFNGLBINDBUFFER_PROC* glpfBindBuffer = NULL;
PFNGLBUFFERDATA_PROC* glpfBufferData = NULL;
PFNGLBUFFERSUBDATA_PROC* glpfBufferSubData = NULL;
PFNGLDELETEBUFFERS_PROC* glpfDeleteBuffers = NULL;
PFNGLDELETEQUERIES_PROC* glpfDeleteQueries = NULL;
PFNGLENDQUERY_PROC* glpfEndQuery = NULL;
PFNGLGENBUFFERS_PROC* glpfGenBuffers = NULL;
PFNGLGENQUERIES_PROC* glpfGenQueries = NULL;
PFNGLGETBUFFERPARAMETERIV_PROC* glpfGetBufferParameteriv = NULL;
PFNGLGETBUFFERPOINTERV_PROC* glpfGetBufferPointerv = NULL;
PFNGLGETBUFFERSUBDATA_PROC* glpfGetBufferSubData = NULL;
PFNGLGETQUERYOBJECTIV_PROC* glpfGetQueryObjectiv = NULL;
PFNGLGETQUERYOBJECTUIV_PROC* glpfGetQueryObjectuiv = NULL;
PFNGLGETQUERYIV_PROC* glpfGetQueryiv = NULL;
PFNGLISBUFFER_PROC* glpfIsBuffer = NULL;
PFNGLISQUERY_PROC* glpfIsQuery = NULL;
PFNGLMAPBUFFER_PROC* glpfMapBuffer = NULL;
PFNGLUNMAPBUFFER_PROC* glpfUnmapBuffer = NULL;

/* GL_VERSION_2_0 */

PFNGLATTACHSHADER_PROC* glpfAttachShader = NULL;
PFNGLBINDATTRIBLOCATION_PROC* glpfBindAttribLocation = NULL;
PFNGLBLENDEQUATIONSEPARATE_PROC* glpfBlendEquationSeparate = NULL;
PFNGLCOMPILESHADER_PROC* glpfCompileShader = NULL;
PFNGLCREATEPROGRAM_PROC* glpfCreateProgram = NULL;
PFNGLCREATESHADER_PROC* glpfCreateShader = NULL;
PFNGLDELETEPROGRAM_PROC* glpfDeleteProgram = NULL;
PFNGLDELETESHADER_PROC* glpfDeleteShader = NULL;
PFNGLDETACHSHADER_PROC* glpfDetachShader = NULL;
PFNGLDISABLEVERTEXATTRIBARRAY_PROC* glpfDisableVertexAttribArray = NULL;
PFNGLDRAWBUFFERS_PROC* glpfDrawBuffers = NULL;
PFNGLENABLEVERTEXATTRIBARRAY_PROC* glpfEnableVertexAttribArray = NULL;
PFNGLGETACTIVEATTRIB_PROC* glpfGetActiveAttrib = NULL;
PFNGLGETACTIVEUNIFORM_PROC* glpfGetActiveUniform = NULL;
PFNGLGETATTACHEDSHADERS_PROC* glpfGetAttachedShaders = NULL;
PFNGLGETATTRIBLOCATION_PROC* glpfGetAttribLocation = NULL;
PFNGLGETPROGRAMINFOLOG_PROC* glpfGetProgramInfoLog = NULL;
PFNGLGETPROGRAMIV_PROC* glpfGetProgramiv = NULL;
PFNGLGETSHADERINFOLOG_PROC* glpfGetShaderInfoLog = NULL;
PFNGLGETSHADERSOURCE_PROC* glpfGetShaderSource = NULL;
PFNGLGETSHADERIV_PROC* glpfGetShaderiv = NULL;
PFNGLGETUNIFORMLOCATION_PROC* glpfGetUniformLocation = NULL;
PFNGLGETUNIFORMFV_PROC* glpfGetUniformfv = NULL;
PFNGLGETUNIFORMIV_PROC* glpfGetUniformiv = NULL;
PFNGLGETVERTEXATTRIBPOINTERV_PROC* glpfGetVertexAttribPointerv = NULL;
PFNGLGETVERTEXATTRIBDV_PROC* glpfGetVertexAttribdv = NULL;
PFNGLGETVERTEXATTRIBFV_PROC* glpfGetVertexAttribfv = NULL;
PFNGLGETVERTEXATTRIBIV_PROC* glpfGetVertexAttribiv = NULL;
PFNGLISPROGRAM_PROC* glpfIsProgram = NULL;
PFNGLISSHADER_PROC* glpfIsShader = NULL;
PFNGLLINKPROGRAM_PROC* glpfLinkProgram = NULL;
PFNGLSHADERSOURCE_PROC* glpfShaderSource = NULL;
PFNGLSTENCILFUNCSEPARATE_PROC* glpfStencilFuncSeparate = NULL;
PFNGLSTENCILMASKSEPARATE_PROC* glpfStencilMaskSeparate = NULL;
PFNGLSTENCILOPSEPARATE_PROC* glpfStencilOpSeparate = NULL;
PFNGLUNIFORM1F_PROC* glpfUniform1f = NULL;
PFNGLUNIFORM1FV_PROC* glpfUniform1fv = NULL;
PFNGLUNIFORM1I_PROC* glpfUniform1i = NULL;
PFNGLUNIFORM1IV_PROC* glpfUniform1iv = NULL;
PFNGLUNIFORM2F_PROC* glpfUniform2f = NULL;
PFNGLUNIFORM2FV_PROC* glpfUniform2fv = NULL;
PFNGLUNIFORM2I_PROC* glpfUniform2i = NULL;
PFNGLUNIFORM2IV_PROC* glpfUniform2iv = NULL;
PFNGLUNIFORM3F_PROC* glpfUniform3f = NULL;
PFNGLUNIFORM3FV_PROC* glpfUniform3fv = NULL;
PFNGLUNIFORM3I_PROC* glpfUniform3i = NULL;
PFNGLUNIFORM3IV_PROC* glpfUniform3iv = NULL;
PFNGLUNIFORM4F_PROC* glpfUniform4f = NULL;
PFNGLUNIFORM4FV_PROC* glpfUniform4fv = NULL;
PFNGLUNIFORM4I_PROC* glpfUniform4i = NULL;
PFNGLUNIFORM4IV_PROC* glpfUniform4iv = NULL;
PFNGLUNIFORMMATRIX2FV_PROC* glpfUniformMatrix2fv = NULL;
PFNGLUNIFORMMATRIX3FV_PROC* glpfUniformMatrix3fv = NULL;
PFNGLUNIFORMMATRIX4FV_PROC* glpfUniformMatrix4fv = NULL;
PFNGLUSEPROGRAM_PROC* glpfUseProgram = NULL;
PFNGLVALIDATEPROGRAM_PROC* glpfValidateProgram = NULL;
PFNGLVERTEXATTRIB1D_PROC* glpfVertexAttrib1d = NULL;
PFNGLVERTEXATTRIB1DV_PROC* glpfVertexAttrib1dv = NULL;
PFNGLVERTEXATTRIB1F_PROC* glpfVertexAttrib1f = NULL;
PFNGLVERTEXATTRIB1FV_PROC* glpfVertexAttrib1fv = NULL;
PFNGLVERTEXATTRIB1S_PROC* glpfVertexAttrib1s = NULL;
PFNGLVERTEXATTRIB1SV_PROC* glpfVertexAttrib1sv = NULL;
PFNGLVERTEXATTRIB2D_PROC* glpfVertexAttrib2d = NULL;
PFNGLVERTEXATTRIB2DV_PROC* glpfVertexAttrib2dv = NULL;
PFNGLVERTEXATTRIB2F_PROC* glpfVertexAttrib2f = NULL;
PFNGLVERTEXATTRIB2FV_PROC* glpfVertexAttrib2fv = NULL;
PFNGLVERTEXATTRIB2S_PROC* glpfVertexAttrib2s = NULL;
PFNGLVERTEXATTRIB2SV_PROC* glpfVertexAttrib2sv = NULL;
PFNGLVERTEXATTRIB3D_PROC* glpfVertexAttrib3d = NULL;
PFNGLVERTEXATTRIB3DV_PROC* glpfVertexAttrib3dv = NULL;
PFNGLVERTEXATTRIB3F_PROC* glpfVertexAttrib3f = NULL;
PFNGLVERTEXATTRIB3FV_PROC* glpfVertexAttrib3fv = NULL;
PFNGLVERTEXATTRIB3S_PROC* glpfVertexAttrib3s = NULL;
PFNGLVERTEXATTRIB3SV_PROC* glpfVertexAttrib3sv = NULL;
PFNGLVERTEXATTRIB4NBV_PROC* glpfVertexAttrib4Nbv = NULL;
PFNGLVERTEXATTRIB4NIV_PROC* glpfVertexAttrib4Niv = NULL;
PFNGLVERTEXATTRIB4NSV_PROC* glpfVertexAttrib4Nsv = NULL;
PFNGLVERTEXATTRIB4NUB_PROC* glpfVertexAttrib4Nub = NULL;
PFNGLVERTEXATTRIB4NUBV_PROC* glpfVertexAttrib4Nubv = NULL;
PFNGLVERTEXATTRIB4NUIV_PROC* glpfVertexAttrib4Nuiv = NULL;
PFNGLVERTEXATTRIB4NUSV_PROC* glpfVertexAttrib4Nusv = NULL;
PFNGLVERTEXATTRIB4BV_PROC* glpfVertexAttrib4bv = NULL;
PFNGLVERTEXATTRIB4D_PROC* glpfVertexAttrib4d = NULL;
PFNGLVERTEXATTRIB4DV_PROC* glpfVertexAttrib4dv = NULL;
PFNGLVERTEXATTRIB4F_PROC* glpfVertexAttrib4f = NULL;
PFNGLVERTEXATTRIB4FV_PROC* glpfVertexAttrib4fv = NULL;
PFNGLVERTEXATTRIB4IV_PROC* glpfVertexAttrib4iv = NULL;
PFNGLVERTEXATTRIB4S_PROC* glpfVertexAttrib4s = NULL;
PFNGLVERTEXATTRIB4SV_PROC* glpfVertexAttrib4sv = NULL;
PFNGLVERTEXATTRIB4UBV_PROC* glpfVertexAttrib4ubv = NULL;
PFNGLVERTEXATTRIB4UIV_PROC* glpfVertexAttrib4uiv = NULL;
PFNGLVERTEXATTRIB4USV_PROC* glpfVertexAttrib4usv = NULL;
PFNGLVERTEXATTRIBPOINTER_PROC* glpfVertexAttribPointer = NULL;

/* GL_VERSION_2_1 */

PFNGLUNIFORMMATRIX2X3FV_PROC* glpfUniformMatrix2x3fv = NULL;
PFNGLUNIFORMMATRIX2X4FV_PROC* glpfUniformMatrix2x4fv = NULL;
PFNGLUNIFORMMATRIX3X2FV_PROC* glpfUniformMatrix3x2fv = NULL;
PFNGLUNIFORMMATRIX3X4FV_PROC* glpfUniformMatrix3x4fv = NULL;
PFNGLUNIFORMMATRIX4X2FV_PROC* glpfUniformMatrix4x2fv = NULL;
PFNGLUNIFORMMATRIX4X3FV_PROC* glpfUniformMatrix4x3fv = NULL;

/* GL_VERSION_3_0 */

PFNGLBEGINCONDITIONALRENDER_PROC* glpfBeginConditionalRender = NULL;
PFNGLBEGINTRANSFORMFEEDBACK_PROC* glpfBeginTransformFeedback = NULL;
PFNGLBINDBUFFERBASE_PROC* glpfBindBufferBase = NULL;
PFNGLBINDBUFFERRANGE_PROC* glpfBindBufferRange = NULL;
PFNGLBINDFRAGDATALOCATION_PROC* glpfBindFragDataLocation = NULL;
PFNGLBINDFRAMEBUFFER_PROC* glpfBindFramebuffer = NULL;
PFNGLBINDRENDERBUFFER_PROC* glpfBindRenderbuffer = NULL;
PFNGLBINDVERTEXARRAY_PROC* glpfBindVertexArray = NULL;
PFNGLBLITFRAMEBUFFER_PROC* glpfBlitFramebuffer = NULL;
PFNGLCHECKFRAMEBUFFERSTATUS_PROC* glpfCheckFramebufferStatus = NULL;
PFNGLCLAMPCOLOR_PROC* glpfClampColor = NULL;
PFNGLCLEARBUFFERFI_PROC* glpfClearBufferfi = NULL;
PFNGLCLEARBUFFERFV_PROC* glpfClearBufferfv = NULL;
PFNGLCLEARBUFFERIV_PROC* glpfClearBufferiv = NULL;
PFNGLCLEARBUFFERUIV_PROC* glpfClearBufferuiv = NULL;
PFNGLCOLORMASKI_PROC* glpfColorMaski = NULL;
PFNGLDELETEFRAMEBUFFERS_PROC* glpfDeleteFramebuffers = NULL;
PFNGLDELETERENDERBUFFERS_PROC* glpfDeleteRenderbuffers = NULL;
PFNGLDELETEVERTEXARRAYS_PROC* glpfDeleteVertexArrays = NULL;
PFNGLDISABLEI_PROC* glpfDisablei = NULL;
PFNGLENABLEI_PROC* glpfEnablei = NULL;
PFNGLENDCONDITIONALRENDER_PROC* glpfEndConditionalRender = NULL;
PFNGLENDTRANSFORMFEEDBACK_PROC* glpfEndTransformFeedback = NULL;
PFNGLFLUSHMAPPEDBUFFERRANGE_PROC* glpfFlushMappedBufferRange = NULL;
PFNGLFRAMEBUFFERRENDERBUFFER_PROC* glpfFramebufferRenderbuffer = NULL;
PFNGLFRAMEBUFFERTEXTURE1D_PROC* glpfFramebufferTexture1D = NULL;
PFNGLFRAMEBUFFERTEXTURE2D_PROC* glpfFramebufferTexture2D = NULL;
PFNGLFRAMEBUFFERTEXTURE3D_PROC* glpfFramebufferTexture3D = NULL;
PFNGLFRAMEBUFFERTEXTURELAYER_PROC* glpfFramebufferTextureLayer = NULL;
PFNGLGENFRAMEBUFFERS_PROC* glpfGenFramebuffers = NULL;
PFNGLGENRENDERBUFFERS_PROC* glpfGenRenderbuffers = NULL;
PFNGLGENVERTEXARRAYS_PROC* glpfGenVertexArrays = NULL;
PFNGLGENERATEMIPMAP_PROC* glpfGenerateMipmap = NULL;
PFNGLGETBOOLEANI_V_PROC* glpfGetBooleani_v = NULL;
PFNGLGETFRAGDATALOCATION_PROC* glpfGetFragDataLocation = NULL;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIV_PROC* glpfGetFramebufferAttachmentParameteriv = NULL;
PFNGLGETINTEGERI_V_PROC* glpfGetIntegeri_v = NULL;
PFNGLGETRENDERBUFFERPARAMETERIV_PROC* glpfGetRenderbufferParameteriv = NULL;
PFNGLGETSTRINGI_PROC* glpfGetStringi = NULL;
PFNGLGETTEXPARAMETERIIV_PROC* glpfGetTexParameterIiv = NULL;
PFNGLGETTEXPARAMETERIUIV_PROC* glpfGetTexParameterIuiv = NULL;
PFNGLGETTRANSFORMFEEDBACKVARYING_PROC* glpfGetTransformFeedbackVarying = NULL;
PFNGLGETUNIFORMUIV_PROC* glpfGetUniformuiv = NULL;
PFNGLGETVERTEXATTRIBIIV_PROC* glpfGetVertexAttribIiv = NULL;
PFNGLGETVERTEXATTRIBIUIV_PROC* glpfGetVertexAttribIuiv = NULL;
PFNGLISENABLEDI_PROC* glpfIsEnabledi = NULL;
PFNGLISFRAMEBUFFER_PROC* glpfIsFramebuffer = NULL;
PFNGLISRENDERBUFFER_PROC* glpfIsRenderbuffer = NULL;
PFNGLISVERTEXARRAY_PROC* glpfIsVertexArray = NULL;
PFNGLMAPBUFFERRANGE_PROC* glpfMapBufferRange = NULL;
PFNGLRENDERBUFFERSTORAGE_PROC* glpfRenderbufferStorage = NULL;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLE_PROC* glpfRenderbufferStorageMultisample = NULL;
PFNGLTEXPARAMETERIIV_PROC* glpfTexParameterIiv = NULL;
PFNGLTEXPARAMETERIUIV_PROC* glpfTexParameterIuiv = NULL;
PFNGLTRANSFORMFEEDBACKVARYINGS_PROC* glpfTransformFeedbackVaryings = NULL;
PFNGLUNIFORM1UI_PROC* glpfUniform1ui = NULL;
PFNGLUNIFORM1UIV_PROC* glpfUniform1uiv = NULL;
PFNGLUNIFORM2UI_PROC* glpfUniform2ui = NULL;
PFNGLUNIFORM2UIV_PROC* glpfUniform2uiv = NULL;
PFNGLUNIFORM3UI_PROC* glpfUniform3ui = NULL;
PFNGLUNIFORM3UIV_PROC* glpfUniform3uiv = NULL;
PFNGLUNIFORM4UI_PROC* glpfUniform4ui = NULL;
PFNGLUNIFORM4UIV_PROC* glpfUniform4uiv = NULL;
PFNGLVERTEXATTRIBI1I_PROC* glpfVertexAttribI1i = NULL;
PFNGLVERTEXATTRIBI1IV_PROC* glpfVertexAttribI1iv = NULL;
PFNGLVERTEXATTRIBI1UI_PROC* glpfVertexAttribI1ui = NULL;
PFNGLVERTEXATTRIBI1UIV_PROC* glpfVertexAttribI1uiv = NULL;
PFNGLVERTEXATTRIBI2I_PROC* glpfVertexAttribI2i = NULL;
PFNGLVERTEXATTRIBI2IV_PROC* glpfVertexAttribI2iv = NULL;
PFNGLVERTEXATTRIBI2UI_PROC* glpfVertexAttribI2ui = NULL;
PFNGLVERTEXATTRIBI2UIV_PROC* glpfVertexAttribI2uiv = NULL;
PFNGLVERTEXATTRIBI3I_PROC* glpfVertexAttribI3i = NULL;
PFNGLVERTEXATTRIBI3IV_PROC* glpfVertexAttribI3iv = NULL;
PFNGLVERTEXATTRIBI3UI_PROC* glpfVertexAttribI3ui = NULL;
PFNGLVERTEXATTRIBI3UIV_PROC* glpfVertexAttribI3uiv = NULL;
PFNGLVERTEXATTRIBI4BV_PROC* glpfVertexAttribI4bv = NULL;
PFNGLVERTEXATTRIBI4I_PROC* glpfVertexAttribI4i = NULL;
PFNGLVERTEXATTRIBI4IV_PROC* glpfVertexAttribI4iv = NULL;
PFNGLVERTEXATTRIBI4SV_PROC* glpfVertexAttribI4sv = NULL;
PFNGLVERTEXATTRIBI4UBV_PROC* glpfVertexAttribI4ubv = NULL;
PFNGLVERTEXATTRIBI4UI_PROC* glpfVertexAttribI4ui = NULL;
PFNGLVERTEXATTRIBI4UIV_PROC* glpfVertexAttribI4uiv = NULL;
PFNGLVERTEXATTRIBI4USV_PROC* glpfVertexAttribI4usv = NULL;
PFNGLVERTEXATTRIBIPOINTER_PROC* glpfVertexAttribIPointer = NULL;

/* GL_VERSION_3_1 */

PFNGLCOPYBUFFERSUBDATA_PROC* glpfCopyBufferSubData = NULL;
PFNGLDRAWARRAYSINSTANCED_PROC* glpfDrawArraysInstanced = NULL;
PFNGLDRAWELEMENTSINSTANCED_PROC* glpfDrawElementsInstanced = NULL;
PFNGLGETACTIVEUNIFORMBLOCKNAME_PROC* glpfGetActiveUniformBlockName = NULL;
PFNGLGETACTIVEUNIFORMBLOCKIV_PROC* glpfGetActiveUniformBlockiv = NULL;
PFNGLGETACTIVEUNIFORMNAME_PROC* glpfGetActiveUniformName = NULL;
PFNGLGETACTIVEUNIFORMSIV_PROC* glpfGetActiveUniformsiv = NULL;
PFNGLGETUNIFORMBLOCKINDEX_PROC* glpfGetUniformBlockIndex = NULL;
PFNGLGETUNIFORMINDICES_PROC* glpfGetUniformIndices = NULL;
PFNGLPRIMITIVERESTARTINDEX_PROC* glpfPrimitiveRestartIndex = NULL;
PFNGLTEXBUFFER_PROC* glpfTexBuffer = NULL;
PFNGLUNIFORMBLOCKBINDING_PROC* glpfUniformBlockBinding = NULL;

/* GL_VERSION_3_2 */

PFNGLCLIENTWAITSYNC_PROC* glpfClientWaitSync = NULL;
PFNGLDELETESYNC_PROC* glpfDeleteSync = NULL;
PFNGLDRAWELEMENTSBASEVERTEX_PROC* glpfDrawElementsBaseVertex = NULL;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEX_PROC* glpfDrawElementsInstancedBaseVertex = NULL;
PFNGLDRAWRANGEELEMENTSBASEVERTEX_PROC* glpfDrawRangeElementsBaseVertex = NULL;
PFNGLFENCESYNC_PROC* glpfFenceSync = NULL;
PFNGLFRAMEBUFFERTEXTURE_PROC* glpfFramebufferTexture = NULL;
PFNGLGETBUFFERPARAMETERI64V_PROC* glpfGetBufferParameteri64v = NULL;
PFNGLGETINTEGER64I_V_PROC* glpfGetInteger64i_v = NULL;
PFNGLGETINTEGER64V_PROC* glpfGetInteger64v = NULL;
PFNGLGETMULTISAMPLEFV_PROC* glpfGetMultisamplefv = NULL;
PFNGLGETSYNCIV_PROC* glpfGetSynciv = NULL;
PFNGLISSYNC_PROC* glpfIsSync = NULL;
PFNGLMULTIDRAWELEMENTSBASEVERTEX_PROC* glpfMultiDrawElementsBaseVertex = NULL;
PFNGLPROVOKINGVERTEX_PROC* glpfProvokingVertex = NULL;
PFNGLSAMPLEMASKI_PROC* glpfSampleMaski = NULL;
PFNGLTEXIMAGE2DMULTISAMPLE_PROC* glpfTexImage2DMultisample = NULL;
PFNGLTEXIMAGE3DMULTISAMPLE_PROC* glpfTexImage3DMultisample = NULL;
PFNGLWAITSYNC_PROC* glpfWaitSync = NULL;

/* GL_VERSION_3_3 */

PFNGLBINDFRAGDATALOCATIONINDEXED_PROC* glpfBindFragDataLocationIndexed = NULL;
PFNGLBINDSAMPLER_PROC* glpfBindSampler = NULL;
PFNGLDELETESAMPLERS_PROC* glpfDeleteSamplers = NULL;
PFNGLGENSAMPLERS_PROC* glpfGenSamplers = NULL;
PFNGLGETFRAGDATAINDEX_PROC* glpfGetFragDataIndex = NULL;
PFNGLGETQUERYOBJECTI64V_PROC* glpfGetQueryObjecti64v = NULL;
PFNGLGETQUERYOBJECTUI64V_PROC* glpfGetQueryObjectui64v = NULL;
PFNGLGETSAMPLERPARAMETERIIV_PROC* glpfGetSamplerParameterIiv = NULL;
PFNGLGETSAMPLERPARAMETERIUIV_PROC* glpfGetSamplerParameterIuiv = NULL;
PFNGLGETSAMPLERPARAMETERFV_PROC* glpfGetSamplerParameterfv = NULL;
PFNGLGETSAMPLERPARAMETERIV_PROC* glpfGetSamplerParameteriv = NULL;
PFNGLISSAMPLER_PROC* glpfIsSampler = NULL;
PFNGLQUERYCOUNTER_PROC* glpfQueryCounter = NULL;
PFNGLSAMPLERPARAMETERIIV_PROC* glpfSamplerParameterIiv = NULL;
PFNGLSAMPLERPARAMETERIUIV_PROC* glpfSamplerParameterIuiv = NULL;
PFNGLSAMPLERPARAMETERF_PROC* glpfSamplerParameterf = NULL;
PFNGLSAMPLERPARAMETERFV_PROC* glpfSamplerParameterfv = NULL;
PFNGLSAMPLERPARAMETERI_PROC* glpfSamplerParameteri = NULL;
PFNGLSAMPLERPARAMETERIV_PROC* glpfSamplerParameteriv = NULL;
PFNGLVERTEXATTRIBDIVISOR_PROC* glpfVertexAttribDivisor = NULL;
PFNGLVERTEXATTRIBP1UI_PROC* glpfVertexAttribP1ui = NULL;
PFNGLVERTEXATTRIBP1UIV_PROC* glpfVertexAttribP1uiv = NULL;
PFNGLVERTEXATTRIBP2UI_PROC* glpfVertexAttribP2ui = NULL;
PFNGLVERTEXATTRIBP2UIV_PROC* glpfVertexAttribP2uiv = NULL;
PFNGLVERTEXATTRIBP3UI_PROC* glpfVertexAttribP3ui = NULL;
PFNGLVERTEXATTRIBP3UIV_PROC* glpfVertexAttribP3uiv = NULL;
PFNGLVERTEXATTRIBP4UI_PROC* glpfVertexAttribP4ui = NULL;
PFNGLVERTEXATTRIBP4UIV_PROC* glpfVertexAttribP4uiv = NULL;

/* GL_VERSION_4_0 */

PFNGLBEGINQUERYINDEXED_PROC* glpfBeginQueryIndexed = NULL;
PFNGLBINDTRANSFORMFEEDBACK_PROC* glpfBindTransformFeedback = NULL;
PFNGLBLENDEQUATIONSEPARATEI_PROC* glpfBlendEquationSeparatei = NULL;
PFNGLBLENDEQUATIONI_PROC* glpfBlendEquationi = NULL;
PFNGLBLENDFUNCSEPARATEI_PROC* glpfBlendFuncSeparatei = NULL;
PFNGLBLENDFUNCI_PROC* glpfBlendFunci = NULL;
PFNGLDELETETRANSFORMFEEDBACKS_PROC* glpfDeleteTransformFeedbacks = NULL;
PFNGLDRAWARRAYSINDIRECT_PROC* glpfDrawArraysIndirect = NULL;
PFNGLDRAWELEMENTSINDIRECT_PROC* glpfDrawElementsIndirect = NULL;
PFNGLDRAWTRANSFORMFEEDBACK_PROC* glpfDrawTransformFeedback = NULL;
PFNGLDRAWTRANSFORMFEEDBACKSTREAM_PROC* glpfDrawTransformFeedbackStream = NULL;
PFNGLENDQUERYINDEXED_PROC* glpfEndQueryIndexed = NULL;
PFNGLGENTRANSFORMFEEDBACKS_PROC* glpfGenTransformFeedbacks = NULL;
PFNGLGETACTIVESUBROUTINENAME_PROC* glpfGetActiveSubroutineName = NULL;
PFNGLGETACTIVESUBROUTINEUNIFORMNAME_PROC* glpfGetActiveSubroutineUniformName = NULL;
PFNGLGETACTIVESUBROUTINEUNIFORMIV_PROC* glpfGetActiveSubroutineUniformiv = NULL;
PFNGLGETPROGRAMSTAGEIV_PROC* glpfGetProgramStageiv = NULL;
PFNGLGETQUERYINDEXEDIV_PROC* glpfGetQueryIndexediv = NULL;
PFNGLGETSUBROUTINEINDEX_PROC* glpfGetSubroutineIndex = NULL;
PFNGLGETSUBROUTINEUNIFORMLOCATION_PROC* glpfGetSubroutineUniformLocation = NULL;
PFNGLGETUNIFORMSUBROUTINEUIV_PROC* glpfGetUniformSubroutineuiv = NULL;
PFNGLGETUNIFORMDV_PROC* glpfGetUniformdv = NULL;
PFNGLISTRANSFORMFEEDBACK_PROC* glpfIsTransformFeedback = NULL;
PFNGLMINSAMPLESHADING_PROC* glpfMinSampleShading = NULL;
PFNGLPATCHPARAMETERFV_PROC* glpfPatchParameterfv = NULL;
PFNGLPATCHPARAMETERI_PROC* glpfPatchParameteri = NULL;
PFNGLPAUSETRANSFORMFEEDBACK_PROC* glpfPauseTransformFeedback = NULL;
PFNGLRESUMETRANSFORMFEEDBACK_PROC* glpfResumeTransformFeedback = NULL;
PFNGLUNIFORM1D_PROC* glpfUniform1d = NULL;
PFNGLUNIFORM1DV_PROC* glpfUniform1dv = NULL;
PFNGLUNIFORM2D_PROC* glpfUniform2d = NULL;
PFNGLUNIFORM2DV_PROC* glpfUniform2dv = NULL;
PFNGLUNIFORM3D_PROC* glpfUniform3d = NULL;
PFNGLUNIFORM3DV_PROC* glpfUniform3dv = NULL;
PFNGLUNIFORM4D_PROC* glpfUniform4d = NULL;
PFNGLUNIFORM4DV_PROC* glpfUniform4dv = NULL;
PFNGLUNIFORMMATRIX2DV_PROC* glpfUniformMatrix2dv = NULL;
PFNGLUNIFORMMATRIX2X3DV_PROC* glpfUniformMatrix2x3dv = NULL;
PFNGLUNIFORMMATRIX2X4DV_PROC* glpfUniformMatrix2x4dv = NULL;
PFNGLUNIFORMMATRIX3DV_PROC* glpfUniformMatrix3dv = NULL;
PFNGLUNIFORMMATRIX3X2DV_PROC* glpfUniformMatrix3x2dv = NULL;
PFNGLUNIFORMMATRIX3X4DV_PROC* glpfUniformMatrix3x4dv = NULL;
PFNGLUNIFORMMATRIX4DV_PROC* glpfUniformMatrix4dv = NULL;
PFNGLUNIFORMMATRIX4X2DV_PROC* glpfUniformMatrix4x2dv = NULL;
PFNGLUNIFORMMATRIX4X3DV_PROC* glpfUniformMatrix4x3dv = NULL;
PFNGLUNIFORMSUBROUTINESUIV_PROC* glpfUniformSubroutinesuiv = NULL;

/* GL_VERSION_4_1 */

PFNGLACTIVESHADERPROGRAM_PROC* glpfActiveShaderProgram = NULL;
PFNGLBINDPROGRAMPIPELINE_PROC* glpfBindProgramPipeline = NULL;
PFNGLCLEARDEPTHF_PROC* glpfClearDepthf = NULL;
PFNGLCREATESHADERPROGRAMV_PROC* glpfCreateShaderProgramv = NULL;
PFNGLDELETEPROGRAMPIPELINES_PROC* glpfDeleteProgramPipelines = NULL;
PFNGLDEPTHRANGEARRAYV_PROC* glpfDepthRangeArrayv = NULL;
PFNGLDEPTHRANGEINDEXED_PROC* glpfDepthRangeIndexed = NULL;
PFNGLDEPTHRANGEF_PROC* glpfDepthRangef = NULL;
PFNGLGENPROGRAMPIPELINES_PROC* glpfGenProgramPipelines = NULL;
PFNGLGETDOUBLEI_V_PROC* glpfGetDoublei_v = NULL;
PFNGLGETFLOATI_V_PROC* glpfGetFloati_v = NULL;
PFNGLGETPROGRAMBINARY_PROC* glpfGetProgramBinary = NULL;
PFNGLGETPROGRAMPIPELINEINFOLOG_PROC* glpfGetProgramPipelineInfoLog = NULL;
PFNGLGETPROGRAMPIPELINEIV_PROC* glpfGetProgramPipelineiv = NULL;
PFNGLGETSHADERPRECISIONFORMAT_PROC* glpfGetShaderPrecisionFormat = NULL;
PFNGLGETVERTEXATTRIBLDV_PROC* glpfGetVertexAttribLdv = NULL;
PFNGLISPROGRAMPIPELINE_PROC* glpfIsProgramPipeline = NULL;
PFNGLPROGRAMBINARY_PROC* glpfProgramBinary = NULL;
PFNGLPROGRAMPARAMETERI_PROC* glpfProgramParameteri = NULL;
PFNGLPROGRAMUNIFORM1D_PROC* glpfProgramUniform1d = NULL;
PFNGLPROGRAMUNIFORM1DV_PROC* glpfProgramUniform1dv = NULL;
PFNGLPROGRAMUNIFORM1F_PROC* glpfProgramUniform1f = NULL;
PFNGLPROGRAMUNIFORM1FV_PROC* glpfProgramUniform1fv = NULL;
PFNGLPROGRAMUNIFORM1I_PROC* glpfProgramUniform1i = NULL;
PFNGLPROGRAMUNIFORM1IV_PROC* glpfProgramUniform1iv = NULL;
PFNGLPROGRAMUNIFORM1UI_PROC* glpfProgramUniform1ui = NULL;
PFNGLPROGRAMUNIFORM1UIV_PROC* glpfProgramUniform1uiv = NULL;
PFNGLPROGRAMUNIFORM2D_PROC* glpfProgramUniform2d = NULL;
PFNGLPROGRAMUNIFORM2DV_PROC* glpfProgramUniform2dv = NULL;
PFNGLPROGRAMUNIFORM2F_PROC* glpfProgramUniform2f = NULL;
PFNGLPROGRAMUNIFORM2FV_PROC* glpfProgramUniform2fv = NULL;
PFNGLPROGRAMUNIFORM2I_PROC* glpfProgramUniform2i = NULL;
PFNGLPROGRAMUNIFORM2IV_PROC* glpfProgramUniform2iv = NULL;
PFNGLPROGRAMUNIFORM2UI_PROC* glpfProgramUniform2ui = NULL;
PFNGLPROGRAMUNIFORM2UIV_PROC* glpfProgramUniform2uiv = NULL;
PFNGLPROGRAMUNIFORM3D_PROC* glpfProgramUniform3d = NULL;
PFNGLPROGRAMUNIFORM3DV_PROC* glpfProgramUniform3dv = NULL;
PFNGLPROGRAMUNIFORM3F_PROC* glpfProgramUniform3f = NULL;
PFNGLPROGRAMUNIFORM3FV_PROC* glpfProgramUniform3fv = NULL;
PFNGLPROGRAMUNIFORM3I_PROC* glpfProgramUniform3i = NULL;
PFNGLPROGRAMUNIFORM3IV_PROC* glpfProgramUniform3iv = NULL;
PFNGLPROGRAMUNIFORM3UI_PROC* glpfProgramUniform3ui = NULL;
PFNGLPROGRAMUNIFORM3UIV_PROC* glpfProgramUniform3uiv = NULL;
PFNGLPROGRAMUNIFORM4D_PROC* glpfProgramUniform4d = NULL;
PFNGLPROGRAMUNIFORM4DV_PROC* glpfProgramUniform4dv = NULL;
PFNGLPROGRAMUNIFORM4F_PROC* glpfProgramUniform4f = NULL;
PFNGLPROGRAMUNIFORM4FV_PROC* glpfProgramUniform4fv = NULL;
PFNGLPROGRAMUNIFORM4I_PROC* glpfProgramUniform4i = NULL;
PFNGLPROGRAMUNIFORM4IV_PROC* glpfProgramUniform4iv = NULL;
PFNGLPROGRAMUNIFORM4UI_PROC* glpfProgramUniform4ui = NULL;
PFNGLPROGRAMUNIFORM4UIV_PROC* glpfProgramUniform4uiv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2DV_PROC* glpfProgramUniformMatrix2dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2FV_PROC* glpfProgramUniformMatrix2fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X3DV_PROC* glpfProgramUniformMatrix2x3dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X3FV_PROC* glpfProgramUniformMatrix2x3fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X4DV_PROC* glpfProgramUniformMatrix2x4dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX2X4FV_PROC* glpfProgramUniformMatrix2x4fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3DV_PROC* glpfProgramUniformMatrix3dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3FV_PROC* glpfProgramUniformMatrix3fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X2DV_PROC* glpfProgramUniformMatrix3x2dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X2FV_PROC* glpfProgramUniformMatrix3x2fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X4DV_PROC* glpfProgramUniformMatrix3x4dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX3X4FV_PROC* glpfProgramUniformMatrix3x4fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4DV_PROC* glpfProgramUniformMatrix4dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4FV_PROC* glpfProgramUniformMatrix4fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X2DV_PROC* glpfProgramUniformMatrix4x2dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X2FV_PROC* glpfProgramUniformMatrix4x2fv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X3DV_PROC* glpfProgramUniformMatrix4x3dv = NULL;
PFNGLPROGRAMUNIFORMMATRIX4X3FV_PROC* glpfProgramUniformMatrix4x3fv = NULL;
PFNGLRELEASESHADERCOMPILER_PROC* glpfReleaseShaderCompiler = NULL;
PFNGLSCISSORARRAYV_PROC* glpfScissorArrayv = NULL;
PFNGLSCISSORINDEXED_PROC* glpfScissorIndexed = NULL;
PFNGLSCISSORINDEXEDV_PROC* glpfScissorIndexedv = NULL;
PFNGLSHADERBINARY_PROC* glpfShaderBinary = NULL;
PFNGLUSEPROGRAMSTAGES_PROC* glpfUseProgramStages = NULL;
PFNGLVALIDATEPROGRAMPIPELINE_PROC* glpfValidateProgramPipeline = NULL;
PFNGLVERTEXATTRIBL1D_PROC* glpfVertexAttribL1d = NULL;
PFNGLVERTEXATTRIBL1DV_PROC* glpfVertexAttribL1dv = NULL;
PFNGLVERTEXATTRIBL2D_PROC* glpfVertexAttribL2d = NULL;
PFNGLVERTEXATTRIBL2DV_PROC* glpfVertexAttribL2dv = NULL;
PFNGLVERTEXATTRIBL3D_PROC* glpfVertexAttribL3d = NULL;
PFNGLVERTEXATTRIBL3DV_PROC* glpfVertexAttribL3dv = NULL;
PFNGLVERTEXATTRIBL4D_PROC* glpfVertexAttribL4d = NULL;
PFNGLVERTEXATTRIBL4DV_PROC* glpfVertexAttribL4dv = NULL;
PFNGLVERTEXATTRIBLPOINTER_PROC* glpfVertexAttribLPointer = NULL;
PFNGLVIEWPORTARRAYV_PROC* glpfViewportArrayv = NULL;
PFNGLVIEWPORTINDEXEDF_PROC* glpfViewportIndexedf = NULL;
PFNGLVIEWPORTINDEXEDFV_PROC* glpfViewportIndexedfv = NULL;

/* GL_VERSION_4_2 */

PFNGLBINDIMAGETEXTURE_PROC* glpfBindImageTexture = NULL;
PFNGLDRAWARRAYSINSTANCEDBASEINSTANCE_PROC* glpfDrawArraysInstancedBaseInstance = NULL;
PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCE_PROC* glpfDrawElementsInstancedBaseInstance = NULL;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCE_PROC* glpfDrawElementsInstancedBaseVertexBaseInstance = NULL;
PFNGLDRAWTRANSFORMFEEDBACKINSTANCED_PROC* glpfDrawTransformFeedbackInstanced = NULL;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCED_PROC* glpfDrawTransformFeedbackStreamInstanced = NULL;
PFNGLGETACTIVEATOMICCOUNTERBUFFERIV_PROC* glpfGetActiveAtomicCounterBufferiv = NULL;
PFNGLGETINTERNALFORMATIV_PROC* glpfGetInternalformativ = NULL;
PFNGLMEMORYBARRIER_PROC* glpfMemoryBarrier = NULL;
PFNGLTEXSTORAGE1D_PROC* glpfTexStorage1D = NULL;
PFNGLTEXSTORAGE2D_PROC* glpfTexStorage2D = NULL;
PFNGLTEXSTORAGE3D_PROC* glpfTexStorage3D = NULL;

/* GL_VERSION_4_3 */

PFNGLBINDVERTEXBUFFER_PROC* glpfBindVertexBuffer = NULL;
PFNGLCLEARBUFFERDATA_PROC* glpfClearBufferData = NULL;
PFNGLCLEARBUFFERSUBDATA_PROC* glpfClearBufferSubData = NULL;
PFNGLCOPYIMAGESUBDATA_PROC* glpfCopyImageSubData = NULL;
PFNGLDEBUGMESSAGECALLBACK_PROC* glpfDebugMessageCallback = NULL;
PFNGLDEBUGMESSAGECONTROL_PROC* glpfDebugMessageControl = NULL;
PFNGLDEBUGMESSAGEINSERT_PROC* glpfDebugMessageInsert = NULL;
PFNGLDISPATCHCOMPUTE_PROC* glpfDispatchCompute = NULL;
PFNGLDISPATCHCOMPUTEINDIRECT_PROC* glpfDispatchComputeIndirect = NULL;
PFNGLFRAMEBUFFERPARAMETERI_PROC* glpfFramebufferParameteri = NULL;
PFNGLGETDEBUGMESSAGELOG_PROC* glpfGetDebugMessageLog = NULL;
PFNGLGETFRAMEBUFFERPARAMETERIV_PROC* glpfGetFramebufferParameteriv = NULL;
PFNGLGETINTERNALFORMATI64V_PROC* glpfGetInternalformati64v = NULL;
PFNGLGETOBJECTLABEL_PROC* glpfGetObjectLabel = NULL;
PFNGLGETOBJECTPTRLABEL_PROC* glpfGetObjectPtrLabel = NULL;
PFNGLGETPOINTERV_PROC* glpfGetPointerv = NULL;
PFNGLGETPROGRAMINTERFACEIV_PROC* glpfGetProgramInterfaceiv = NULL;
PFNGLGETPROGRAMRESOURCEINDEX_PROC* glpfGetProgramResourceIndex = NULL;
PFNGLGETPROGRAMRESOURCELOCATION_PROC* glpfGetProgramResourceLocation = NULL;
PFNGLGETPROGRAMRESOURCELOCATIONINDEX_PROC* glpfGetProgramResourceLocationIndex = NULL;
PFNGLGETPROGRAMRESOURCENAME_PROC* glpfGetProgramResourceName = NULL;
PFNGLGETPROGRAMRESOURCEIV_PROC* glpfGetProgramResourceiv = NULL;
PFNGLINVALIDATEBUFFERDATA_PROC* glpfInvalidateBufferData = NULL;
PFNGLINVALIDATEBUFFERSUBDATA_PROC* glpfInvalidateBufferSubData = NULL;
PFNGLINVALIDATEFRAMEBUFFER_PROC* glpfInvalidateFramebuffer = NULL;
PFNGLINVALIDATESUBFRAMEBUFFER_PROC* glpfInvalidateSubFramebuffer = NULL;
PFNGLINVALIDATETEXIMAGE_PROC* glpfInvalidateTexImage = NULL;
PFNGLINVALIDATETEXSUBIMAGE_PROC* glpfInvalidateTexSubImage = NULL;
PFNGLMULTIDRAWARRAYSINDIRECT_PROC* glpfMultiDrawArraysIndirect = NULL;
PFNGLMULTIDRAWELEMENTSINDIRECT_PROC* glpfMultiDrawElementsIndirect = NULL;
PFNGLOBJECTLABEL_PROC* glpfObjectLabel = NULL;
PFNGLOBJECTPTRLABEL_PROC* glpfObjectPtrLabel = NULL;
PFNGLPOPDEBUGGROUP_PROC* glpfPopDebugGroup = NULL;
PFNGLPUSHDEBUGGROUP_PROC* glpfPushDebugGroup = NULL;
PFNGLSHADERSTORAGEBLOCKBINDING_PROC* glpfShaderStorageBlockBinding = NULL;
PFNGLTEXBUFFERRANGE_PROC* glpfTexBufferRange = NULL;
PFNGLTEXSTORAGE2DMULTISAMPLE_PROC* glpfTexStorage2DMultisample = NULL;
PFNGLTEXSTORAGE3DMULTISAMPLE_PROC* glpfTexStorage3DMultisample = NULL;
PFNGLTEXTUREVIEW_PROC* glpfTextureView = NULL;
PFNGLVERTEXATTRIBBINDING_PROC* glpfVertexAttribBinding = NULL;
PFNGLVERTEXATTRIBFORMAT_PROC* glpfVertexAttribFormat = NULL;
PFNGLVERTEXATTRIBIFORMAT_PROC* glpfVertexAttribIFormat = NULL;
PFNGLVERTEXATTRIBLFORMAT_PROC* glpfVertexAttribLFormat = NULL;
PFNGLVERTEXBINDINGDIVISOR_PROC* glpfVertexBindingDivisor = NULL;

/* GL_VERSION_4_4 */

PFNGLBINDBUFFERSBASE_PROC* glpfBindBuffersBase = NULL;
PFNGLBINDBUFFERSRANGE_PROC* glpfBindBuffersRange = NULL;
PFNGLBINDIMAGETEXTURES_PROC* glpfBindImageTextures = NULL;
PFNGLBINDSAMPLERS_PROC* glpfBindSamplers = NULL;
PFNGLBINDTEXTURES_PROC* glpfBindTextures = NULL;
PFNGLBINDVERTEXBUFFERS_PROC* glpfBindVertexBuffers = NULL;
PFNGLBUFFERSTORAGE_PROC* glpfBufferStorage = NULL;
PFNGLCLEARTEXIMAGE_PROC* glpfClearTexImage = NULL;
PFNGLCLEARTEXSUBIMAGE_PROC* glpfClearTexSubImage = NULL;



static void add_extension(const char* extension)
{
    if (strcmp("GL_ARB_bindless_texture", extension) == 0) {
        FLEXT_ARB_bindless_texture = GL_TRUE;
    }
    if (strcmp("GL_EXT_direct_state_access", extension) == 0) {
        FLEXT_EXT_direct_state_access = GL_TRUE;
    }
    if (strcmp("GL_KHR_texture_compression_astc_ldr", extension) == 0) {
        FLEXT_KHR_texture_compression_astc_ldr = GL_TRUE;
    }
    if (strcmp("GL_KHR_texture_compression_astc_hdr", extension) == 0) {
        FLEXT_KHR_texture_compression_astc_hdr = GL_TRUE;
    }
    if (strcmp("GL_ARB_direct_state_access", extension) == 0) {
        FLEXT_ARB_direct_state_access = GL_TRUE;
    }
}


/* ------------------ get_proc from Slavomir Kaslev's gl3w ----------------- */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

static HMODULE libgl;

static void open_libgl(void)
{
    libgl = LoadLibraryA("opengl32.dll");
}

static void close_libgl(void)
{
    FreeLibrary(libgl);
}

static GLPROC get_proc(const char *proc)
{
    GLPROC res;

    res = wglGetProcAddress(proc);
    if (!res)
        res = GetProcAddress(libgl, proc);
    return res;
}
#elif defined(__APPLE__) || defined(__APPLE_CC__)
#include <Carbon/Carbon.h>

CFBundleRef bundle;
CFURLRef bundleURL;

static void open_libgl(void)
{
    bundleURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                CFSTR("/System/Library/Frameworks/OpenGL.framework"),
                kCFURLPOSIXPathStyle, true);

    bundle = CFBundleCreate(kCFAllocatorDefault, bundleURL);
    assert(bundle != NULL);
}

static void close_libgl(void)
{
    CFRelease(bundle);
    CFRelease(bundleURL);
}

static GLPROC get_proc(const char *proc)
{
    GLPROC res;

    CFStringRef procname = CFStringCreateWithCString(kCFAllocatorDefault, proc,
                kCFStringEncodingASCII);
    FLEXT_C_EXTENSION(res = CFBundleGetFunctionPointerForName(bundle, procname));
    CFRelease(procname);
    return res;
}
#elif defined(ANDROID)
#include <EGL/egl.h>

static void open_libgl(void)
{
    // nothing to do
}

static void close_libgl(void)
{
    // nothing to do
}

static GLPROC get_proc(const char *proc)
{
    GLPROC res;
    res = eglGetProcAddress((const char *) proc);
    return res;
}
#else
#include <dlfcn.h>
#include <GL/glx.h>

static void *libgl;

static void open_libgl(void)
{
    libgl = dlopen("libGL.so.1", RTLD_LAZY | RTLD_GLOBAL);
}

static void close_libgl(void)
{
    dlclose(libgl);
}

static GLPROC get_proc(const char *proc)
{
    GLPROC res;

    res = glXGetProcAddress((const GLubyte *) proc);
    if (!res) {
        FLEXT_C_EXTENSION(res = dlsym(libgl, proc));
    }
    return res;
}
#endif

#ifdef __cplusplus
}
#endif
