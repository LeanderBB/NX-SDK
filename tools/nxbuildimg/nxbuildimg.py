#!/usr/bin/env python
# -*- coding: utf-8 -*-

#
# nxbuildimg.py
# Process input image file with ImageMagicks convert, generate all possible
# mipmap sizes, encoded them using the astc encoder and generate a nximg file.
#
# Environment variables required:
# NX_BUILDIMG_CONVERT       - Location of imagemagick's convert
# NX_BUILDIMG_IDENTIFY      - Location of imagemagick's identify
# NX_BUILDIMG_ASTCENC       - Location of Mali's texture tools astcenc
# NX_BUILDIMG_NXASTCTONXIMG - Location of nxastctonximg tool
#

# convert  -size XxY input output
# identify -format "%[fx:w] %[fx:h]\n"
#
#

import re
import os
import sys
import subprocess

prog_convert = None
prog_identify = None
prog_astcenc = None
prog_nxastctonximg = None

def test_env():
    ''' Test environment, check if all data is set'''

    global prog_convert
    global prog_identify
    global prog_astcenc
    global prog_nxastctonximg

    prog_convert = os.environ.get('NX_BUILDIMG_CONVERT')
    if (prog_convert == None):
        raise Exception("NX_BUILDIMG_CONVERT is not set")

    prog_identify = os.environ.get('NX_BUILDIMG_IDENTIFY')
    if (prog_identify == None):
        raise Exception("NX_BUILDIMG_IDENTIFY is not set")

    prog_astcenc = os.environ.get('NX_BUILDIMG_ASTCENC')
    if (prog_astcenc == None):
        raise Exception("NX_BUILDIMG_ASTCENC is not set")

    prog_nxastctonximg = os.environ.get('NX_BUILDIMG_NXASTCTONXIMG')
    if (prog_nxastctonximg == None):
        raise Exception("NX_BUILDIMG_NXASTCTONXIMG is not set")

    print "NX_BUILDIMG_CONVERT       -", prog_convert
    print "NX_BUILDIMG_IDENTIFY      -", prog_identify
    print "NX_BUILDIMG_ASTCENC       -", prog_astcenc
    print "NX_BUILDIMG_NXASTCTONXIMG -", prog_nxastctonximg


def get_image_info(path):
    ''' Get the image info using image magick's identify
        returns(-1, -1, -1, False) on failure, or (w, h, channeld depth, srgb) on success '''
    global prog_identify

    # run the identify
    args = [prog_identify,'-format', '%[fx:w] %[fx:h] %[fx:z]', path]
    string = subprocess.check_output(args)

    # check if the result is valid
    pattern = re.compile(r"(\d+) (\d+) (\d+)")
    m = pattern.match(string)
    grps = m.groups()
    if m == None or len(grps) < 3:
        return (-1, -1, -1, False)


    # check if it has srgb
    args = [prog_identify,'-format', '%r', path]
    string = subprocess.check_output(args)

    srgb = False
    if "sRGB" in string:
        srgb = True

    return (int(grps[0]), int(grps[1]), int(grps[2]), srgb)

def get_file_name(path):
    ''' Return the filename without extension '''
    basename = os.path.basename(path)
    tuple = os.path.splitext(basename)
    return tuple[0]


def gen_mip_maps(input, outputdir):
    '''
        Generate all possible mipmaps for the input file in the outputdir
        Returns list of generated files
    '''

    global prog_convert

    file_name = get_file_name(input)
    img_size = get_image_info(input)

    if (img_size[0] == -1 or img_size[1] == -1):
        raise Exception("Failed to retrieve image size for " + input)


    width = img_size[0]
    height = img_size[1]
    level = 0
    file_names = []
    # generate mipmaps
    while width != 1 and height != 1:
        output_path = os.path.join(outputdir, file_name + '_' + str(level) + '_.tga')
        try:
            subprocess.check_call([prog_convert, input, '-resize', str(width) + 'x' + str(height), output_path])
        except subprocess.CalledProcessError:
            raise Exception("Failed to convert level " + str(level) + "for " + input)

        file_names.append(output_path)

        width = max(1, width >> 1)
        height = max(1, height >> 1)
        level = level + 1

    return (img_size, file_names)

def gen_astc(output, mip_result):
    '''
        Convert mipmpas to astc images
        Returns list of converted files
    '''
    global prog_astcenc
    files = mip_result[1]
    info = mip_result[0]

    output_files = []
    for file in files:
        out_name = os.path.join(output,get_file_name(file) + ".astc")
        args = [prog_astcenc, '-c' if not info[3] else '-cs' , file, out_name , "4x4", "-thorough", '-silentmode']
        try:
            subprocess.check_call(args)
        except subprocess.CalledProcessError:
            raise Exception("Failed to generate astc for " + file)

        output_files.append(out_name)

    return output_files


def gen_nximg(output, mip_result, astc_files):
    ''' Generate nximg '''
    global prog_nxastctonximg

    info = mip_result[0]
    info_str = ('0' if (info[2] < 10) else '') + str(info[2]) + ":" + ( '1' if info[3] else '0') + ":04x04"

    args = [prog_nxastctonximg, output, info_str]

    for file in astc_files:
        args.append(file)

    try:
        subprocess.check_call(args)
    except subprocess.CalledProcessError:
        raise Exception("Failed to generate nximg")


def main():
    try:
        test_env()
        print ""
        res = gen_mip_maps(sys.argv[1],sys.argv[2])
        res_astc = gen_astc(sys.argv[2], res)
        gen_nximg("test.nximg", res, res_astc)
        sys.exit(0)
    except Exception as e:
        print "Exception:", e
        sys.exit(1)


if __name__ == '__main__':
    main()
