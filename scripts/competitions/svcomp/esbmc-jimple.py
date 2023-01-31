#!/usr/bin/env python3

import logging
import argparse
import pathlib
import tempfile
import shutil
import os
from subprocess import Popen, PIPE

# Thirdy-party
import yaml  # TODO: remove this after integrating with benchexec


class Config:
    DEBUG = False
    TMPFOLDER_PREFIX = "ESBMC-Jimple-"
    BINARIES = {"_str_": pathlib.Path("_str_")}
    # TODO: Add logger here


def parse_yaml(input: str):
    logging.info(f"Parsing input {input}")
    path = pathlib.Path(input)
    assert (path.is_file())
    with open(path, "r") as f:
        contents = yaml.safe_load(f)
        if (Config.DEBUG):
            logging.debug(f"Parsed YAML: {contents}")
        return contents


def arg_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument('input', type=str)
    parser.add_argument("--debug", action=argparse.BooleanOptionalAction)
    args = parser.parse_args()

    # Validation
    if (not pathlib.Path(args.input).is_file()):
        logging.error(f"Input {args.input} not found")

    Config.DEBUG = args.debug
    return args


def preconditions():
    # Checks whether everything needed is available
    for e in ["javac", "jimple2json", "java"]:
        Config.BINARIES[e] = shutil.which(e)

    # TODO: Add soot and esbmc to path
    Config.BINARIES["soot"] = pathlib.Path(
        "/Users/user/Repos/esbmc/scripts/competitions/svcomp/soot.jar")
    Config.BINARIES["verifier"] = pathlib.Path(
        "/Users/user/Repos/esbmc/scripts/competitions/svcomp/Verifier.java")
    Config.BINARIES["esbmc"] = shutil.which(
        "/Users/user/Repos/esbmc/build/src/esbmc/esbmc")

    error = False
    # TODO: Move this to the previous loop
    for e in ["javac", "jimple2json", "java", "esbmc", "soot", "verifier"]:
        if not Config.BINARIES[e]:
            logging.warning(f"Could not find {e}")
            error = True
        else:
            logging.debug(f"Using {e}: {Config.BINARIES[e]}")

    if (error):
        logging.error("Failed precondition checks")
        raise ValueError()


# From .java to .class
def compile_inputs(folder, inputs):
    parameters = [Config.BINARIES["javac"], "-d", folder]
    for i in inputs:
        logging.debug(f"Parameter {i}")
        if (i == "../common/"):
            parameters.append(Config.BINARIES["verifier"])
            continue
        for x in pathlib.Path(i).glob('**/*'):
            parameters.append(x)
    logging.debug(f"Parameters: {parameters}")
    process = Popen(parameters, stdout=PIPE, stderr=PIPE)
    _, stderr = process.communicate()

    if stderr:
        logging.error(stderr)


def convert_to_jimple(folder):
    inputs = []
    for x in pathlib.Path(folder).glob('**/*.class'):
        i = str(x)[len(folder)+1:len(str(x)) - len(".class")]
        if i == "org/sosy_lab/sv_benchmarks/Verifier":
            continue
        logging.debug(f"Converting .class: {i}")
        inputs.append(i)

    def convert_file(f):
        parameters = [Config.BINARIES["java"], "-cp", Config.BINARIES["soot"], "soot.Main", "-cp", ".", "-pp", "-f", "jimple",
                      f, "-write-local-annotations", "-p", "jb", "use-original-names:true", "-keep-line-number", "-print-tags-in-output"]
        logging.debug(f"Running soot: {parameters}")
        process = Popen(parameters, stdout=PIPE, stderr=PIPE, cwd=folder)
        _, stderr = process.communicate()

        if stderr:
            logging.error(stderr)
            err_process = Popen(["ls"], stdout=PIPE, stderr=PIPE, cwd=folder)
            stdout, stderr = err_process.communicate()
            logging.debug(stdout)

    for i in inputs:
        convert_file(i)


def run_esbmc(folder, expected=True):
    parameters = [Config.BINARIES["esbmc"], "--force-malloc-success",
                  "--k-induction", "--function", "main_1"]
    for i in pathlib.Path(folder).glob('**/*.jimple'):
        parameters.append(i)

    logging.debug(f"Running esbmc with {parameters}")
    process = Popen(parameters, stdout=PIPE, stderr=PIPE)
    stdout, stderr = process.communicate()
    if (stderr):
        logging.error(stderr)

    logging.info(f"Expected: {expected}")
    if ("VERIFICATION SUCCESSFUL" in str(stdout)):
        logging.info("True")
        return 1
    elif ("VERIFICATION FAILED" in str(stdout)):
        logging.info("False")
        return -1
    else:
        logging.info("Unknown")
        return 0




def main():
    # Set arguments
    args = arg_parser()

    # Configure Logger
    format_str = '%(levelname)s - [%(funcName)s] %(message)s'
    logging.basicConfig(format=format_str,
                        level=logging.DEBUG if Config.DEBUG else logging.info)
    logging.info("Started ESBMC-Jimple")
    logging.debug("Enabled DEBUG mode")

    # Check preconditions
    preconditions()

    # Parse Inputs
    yml = parse_yaml(args.input)

    # Main Flow:
    #   1. Compile programs using inputs (.java) into .class files.
    #   2. Convert .class files into .jimple
    #   3. Run ESBMC
    #   4. Fetch Results (optional: validate them)
    with tempfile.TemporaryDirectory(prefix=Config.TMPFOLDER_PREFIX) as tmpdirname:
        logging.debug(f"Created temporary directory: {tmpdirname}")
        compile_inputs(tmpdirname, yml['input_files'])
        convert_to_jimple(tmpdirname)
        run_esbmc(tmpdirname, yml["properties"][0]["expected_verdict"])


if __name__ == "__main__":
    main()
