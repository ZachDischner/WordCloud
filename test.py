#!/usr/bin/env python
from __future__ import print_function

"""
Very quick script to test the execution of compiled 'WordCloud' binary 
against sample datasets

Not intended to be a comprehensive validator, just a first step sanity check.

Just run from the command line and verify that everything worked as intended.
"""

###### Imports
import subprocess   # The 'correct' way to test command line programs
import os           # The 'correct' way makes it difficult to test --options. This is just plain easy

def dict_from_output(output):
    return {res.split(" ")[0]:int(res.split(" ")[1]) for res in output.split("\n")[1:-1]}

def test_basic():
    spec = {"five":5, "four":4, "three":3, "two":2, "one":1}
    input_str = " ".join([(k+" ") * v for k,v in spec.items()])
    print("\n\nTesting basic functionality agains input string: '{}'".format(input_str))
    output = subprocess.check_output(["./WordCloud",input_str])
    keys = [o.split("\n")[-1] for o in output.split(" ")][:-1]
    assert sorted(keys) == keys, "Output words are not alphabetical!"
    results = dict_from_output(output)
    for k,v in spec.items():
        assert results[k] == spec[k], "Expected {} counts of word '{}', only got {}!".format(v,k,results[k])

    assert os.popen("./WordCloud 'a am an as me I do'").read() == "\n", "String with BS words made it through into the word cloud!"
    print("Basic test passed!")
    return True

def test_funky():
    print("\n\nTesting parsing of sample file 'test.txt' with some arbritrary test cases thrown in")
    output = subprocess.check_output(["./WordCloud", "test.txt"])
    results = dict_from_output(output)
    assert "error" not in results.keys(), "All 'error' words should have not made it into the word cloud, but {} did!".format(results["error"])
    print("'test.txt' test passed!")
    return True

def test_options():
    print("\n\nTesting against optional keyword arguments")
    output = os.popen("./WordCloud randomWords.txt --mincounts=2 --maxcounts=5").read()
    results = dict_from_output(output)
    assert False not in [2 <= v <= 5 for v in results.values()], "Counts of words did not all fall within the 2-5 min/max range when called with --mincounts=2 and --maxcounts=5!\n{}".format(output)

    output = os.popen("./WordCloud randomWords.txt --startswith='ab'").read()
    results = dict_from_output(output)
    assert False not in [k.startswith("ab") for k in results.keys()], "Words output were not limited to specified --startswith='ab' condition!\n{}".format(output)
    return True



if __name__ == "__main__":
    print("Running test cases against WordCloud compiled binary")
    for func in [test_basic, test_funky, test_options]:
        print("\t>>Test `{}()` passed? {}".format(func.func_name,func()))
