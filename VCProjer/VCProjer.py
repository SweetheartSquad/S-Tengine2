__author__ = 'ryan'

import sys
import re

print(sys.argv)
if len(sys.argv) >= 3:
    mainString = None
    try:
        with open(sys.argv[1], "rb") as mainFile:
            mainString = mainFile.read()
    except IOError:
        if mainString is None:
            print "Problem occurred when reading file " + sys.argv[1]
            exit()

    includeRegex = re.compile(ur'(<ClInclude Include(.*/>\r\n)*)')
    compileRegex = re.compile(ur'(<ClCompile Include(.*/>\r\n)*)')

    mainString = mainString.replace("\\", "/")

    includeMatch = includeRegex.search(mainString)
    compileMatch = compileRegex.search(mainString)

    includeString = includeMatch.group(1)
    compileString = compileMatch.group(1)

    mainFiltersString = None
    try:
        with open(sys.argv[1] + ".filters", "r") as mainFiltersFile:
            mainFiltersString = mainFiltersFile.read()
    except IOError:
        if mainFiltersString is None:
            print "Problem occurred when reading file " + sys.argv[1] + ".filters"

    for f in range(2, len(sys.argv)):
        tempFileString = None
        try:
            with open(sys.argv[f], "r+b") as tempFile:
                tempFileString = tempFile.read()
                tempFileString = re.sub(includeRegex, includeString, tempFileString)
                tempFileString = re.sub(compileRegex, compileString, tempFileString)
                tempFile.seek(0)
                tempFile.write(tempFileString)
                tempFile.truncate()
        except IOError:
            if tempFileString is None:
                print "Problem occurred when reading file " + sys.argv[f]
                exit()

        if mainFiltersString is not None:
            with open(sys.argv[int(f)] + ".filters", "wb") as tempFiltersFile:
                tempFiltersFile.seek(0)
                tempFiltersFile.write(mainFiltersString)
                tempFiltersFile.truncate()

else:
    print "Expected at least two arguments"
