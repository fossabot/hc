#!/usr/bin/python2.7
# coding=windows-1251
import logging
import os
import shutil
import sys
import subprocess
import argparse
from lxml import etree
import binascii

__author__ = 'Alexander Egorov'

result_dir = 'hql'

def CreateQueryFromTridXml(path):
    logging.info("processing %s", path)
    title = ''
    signature_ext = ''
    descr = ''
    bytes = ''
    offset = 0
    file_prefix = '__test_'
    with open(path, 'r') as f:
        list = []
        patterns = {}
        ix = 0
        for event, element in etree.iterparse(f, events=("start", "end")):
            if event == 'start':
                if element.tag == 'FileType':
                    title = element.text
                if element.tag == 'Ext':
                    signature_ext = element.text
                if element.tag == 'Rem':
                    descr = element.text
                if element.tag == 'Bytes':
                    bytes = element.text
                if element.tag == 'Pos':
                    offset = int(element.text)
            if event == 'end':
                if element.tag == 'FrontBlock':
                    break
                if element.tag == 'Pattern':
                    binary = binascii.unhexlify(bytes)
                    tmp_file = "{0}{1:4d}.bin".format(file_prefix, ix)
                    ix += 1
                    patterns[tmp_file] = offset, len(bytes) / 2
                    with open(tmp_file, "wb") as tmp:
                        tmp.write(binary)

        try:
            s = subprocess.check_output("md5 -d . -i {0}*.bin".format(file_prefix))
            lines = s.split('\n')
            for line in lines:
                if len(line) > 1:
                    pieces = line.split('|')
                    dir, name = os.path.split(pieces[0].strip())

                    hash = pieces[2].strip()
                    list.append(
                        "(f.offset == {0:d} and f.limit == {1:d} and f.md5 == '{2}')".format(patterns[name][0], patterns[name][1], hash))
        finally:
            for p in patterns.iterkeys():
                os.remove(p)

        where = ' and\n'.join(list)
        if descr is None:
            descr = ''
        q = "# {0} ({1})\n# {2}\n\nfor file f from parameter where\n{3}\ndo validate;".format(title, signature_ext, descr, where)

        dir, name = os.path.split(path)
        root, ext = os.path.splitext(name)
        file_path = os.path.join(result_dir, "{0}.hql".format(root))
        with open(file_path, "w") as qf:
            qf.write(q)


def main():
    logging.basicConfig(format=("%(asctime).19s %(levelname)s %(message)s "))

    parser = argparse.ArgumentParser(description="TRiD signatures converting tool. Copyright (C) 2012 Alexander Egorov.")
    parser.add_argument('-p', '--path', dest='path', help='Path to TRiD signature files', required=True)
    parser.add_argument('-v', '--verbose', dest='verbose', help='Verbose output', action='store_true', default=False)

    args = parser.parse_args()

    if args.verbose:
        logging.getLogger().setLevel(logging.DEBUG)
    else:
        logging.getLogger().setLevel(logging.INFO)

    files = os.listdir(args.path)

    if os.path.isdir(result_dir):
        shutil.rmtree(result_dir, True)
    os.mkdir(result_dir)
    for filename in files:
        if filename.rfind(".trid.xml") == -1:
            continue
        CreateQueryFromTridXml(os.path.join(args.path, filename))

if __name__ == '__main__':
    sys.exit(main())