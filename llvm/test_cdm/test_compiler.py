from enum import IntEnum
import json
import os
from pathlib import Path
import subprocess
import pytest


class ReturnCode(IntEnum):
    SUCCESS = 0
    ERROR = 1
    TIMEOUT = 13


def assert_nested_subset(expected, actual, path=""):
    if isinstance(expected, dict):
        assert isinstance(
            actual, dict
        ), f"Type mismatch at {path}: expected dict, got {type(actual).__name__}"
        for key, value in expected.items():
            assert key in actual, f"Missing key '{key}' at {path}"
            assert_nested_subset(value, actual[key], path + f".{key}" if path else key)
    elif isinstance(expected, list):
        assert isinstance(
            actual, list
        ), f"Type mismatch at {path}: expected list, got {type(actual).__name__}"
        assert len(expected) == len(
            actual
        ), f"Length mismatch at {path}: expected {len(expected)}, got {len(actual)}"
        for i, (exp_item, act_item) in enumerate(zip(expected, actual)):
            assert_nested_subset(exp_item, act_item, path + f"[{i}]")
    else:
        assert (
            expected == actual
        ), f"Mismatch at {path}: expected {expected}, got {actual}"


def run_test(
    expected, optimisation_level, src_dir, gen_dir, bin_dir, build_dir, resources, ivt
):
    src_file = src_dir / f"{expected.stem}.c"
    try:
        assert src_file.exists(), f"Source file not found: {src_file}"

        asm_res = gen_dir / f"{expected.stem}_O{optimisation_level}.asm"
        asm_gen_process = subprocess.run(
            [
                "./clang",
                "-g",
                "-target",
                "cdm",
                "-S",
                f"-O{optimisation_level}",
                "-I",
                "../../test_cdm/libs/inc",
                str(src_file),
                "-o",
                str(asm_res),
            ],
            cwd=str(bin_dir),
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        assert asm_gen_process.returncode == ReturnCode.SUCCESS, (
            f"ASM generation failed for {src_file} with return code {asm_gen_process.returncode}.\n"
            f"Stdout: {asm_gen_process.stdout.decode()}\n"
            f"Stderr: {asm_gen_process.stderr.decode()}"
        )

        bin_res = gen_dir / f"{expected.stem}_O{optimisation_level}.img"
        bin_gen_process = subprocess.run(
            [
                "docker",
                "run",
                "--rm",
                "-v",
                f"{build_dir}:/root",
                "-v",
                f"{ivt}:/root/ivt.img",
                "cdm-devkit",
                "cocas",
                "-t",
                "cdm16",
                "-o",
                f"/root/gen/{bin_res.name}",
                "/root/ivt.img",
                "/root/libs/ctype.asm", # todo: automize library list
                "/root/libs/string.asm",
                f"/root/gen/{asm_res.name}",
            ],
            cwd=str(bin_dir),
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        assert bin_gen_process.returncode == ReturnCode.SUCCESS, (
            f"Binary generation failed for {src_file} with return code {bin_gen_process.returncode}.\n"
            f"Stdout: {bin_gen_process.stdout.decode()}\n"
            f"Stderr: {bin_gen_process.stderr.decode()}"
        )

        mem_res = gen_dir / f"{expected.stem}_O{optimisation_level}_mem.img"
        run_process = subprocess.run(
            [
                "docker",
                "run",
                "--rm",
                "-v",
                f"{build_dir}:/root",
                "--workdir",
                f"/root/{os.path.relpath(resources, build_dir)}",
                  "cdm-devkit",
                "java",
                "-jar",
                "logisim-runner-all.jar",
                f"/root/{os.path.relpath(bin_res, build_dir)}",
                "test_circuit_emu.circ",
                f"/root/{os.path.relpath(mem_res, build_dir)}",
                "config.properties",
                "10000",
            ],
            cwd=resources,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )

        assert run_process.returncode == ReturnCode.SUCCESS, (
            f"Simulation failed for {src_file} with return code {run_process.returncode}.\n"
            f"Stdout: {run_process.stdout.decode()}\n"
            f"Stderr: {run_process.stderr.decode()}"
        )

        output = json.loads(run_process.stdout)
        output["memory"] = json.loads(Path(mem_res).read_text("UTF-8"))
        data_expected = json.loads(Path(expected).read_text("UTF-8"))

        assert_nested_subset(data_expected, output)
    except AssertionError as e:
        e.add_note(
            f"error at test {expected.stem} with optimization O{optimisation_level}"
        )
        raise


def get_test_cases():
    test_dir = Path(__file__).parent.absolute()
    output_dir = test_dir / "output"
    src_dir = test_dir / "src"
    build_dir = test_dir / "build"
    gen_dir = build_dir / "gen"
    bin_dir = (test_dir / ".." / "build" / "bin").absolute()
    resources = build_dir / "resources"
    ivt = test_dir / "ivt.asm"

    cases = []
    for expected in filter(Path.is_file, output_dir.iterdir()):
        for optimization_level in ["0", "1", "2", "3", "s"]:
            cases.append(
                (
                    expected,
                    optimization_level,
                    src_dir,
                    gen_dir,
                    bin_dir,
                    build_dir,
                    resources,
                    ivt,
                )
            )
    return cases


@pytest.mark.parametrize(
    "expected, optimisation_level, src_dir, gen_dir, bin_dir, build_dir, resources, ivt",
    get_test_cases(),
)
def test_compiler(
    expected, optimisation_level, src_dir, gen_dir, bin_dir, build_dir, resources, ivt
):
    run_test(
        expected,
        optimisation_level,
        src_dir,
        gen_dir,
        bin_dir,
        build_dir,
        resources,
        ivt,
    )


if __name__ == "__main__":
    import sys

    sys.exit(pytest.main([__file__]))
