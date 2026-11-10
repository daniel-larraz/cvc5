#!/usr/bin/env python3
# TEMPORARY: measure the effect of running run_regression.py with the base
# Python interpreter, with -S, and without the dump tester, on the same runner
# and the same tests.
# usage: compare3.py <build-dir> <label>
import glob, json, os, re, shutil, statistics, subprocess, sys, time

build, label = os.path.abspath(sys.argv[1]), sys.argv[2]
work = os.path.join(build, "timing-variants")
shutil.rmtree(work, ignore_errors=True)
os.makedirs(work)

info = json.loads(subprocess.run(
    ["ctest", "--show-only=json-v1", "-L", "regress0"], cwd=build,
    capture_output=True, text=True, check=True).stdout)
tests = [t for t in info["tests"] if t["name"].startswith("regress0/")]
venv_python = (glob.glob(os.path.join(build, "venv-*", "Scripts", "python.exe"))
               + glob.glob(os.path.join(build, "venv-*", "bin", "python3")))[0]
base_python = tests[0]["command"][0]
assert tests[0]["command"][1] == "-S", tests[0]["command"][:3]
print("regress0 tests:", len(tests))
print("venv python:", venv_python)
print("base python:", base_python)


def prop(t, name):
    for p in t.get("properties", []):
        if p["name"] == name:
            return p["value"]
    return None


def bracket(s):
    assert "]==]" not in s
    return "[==[" + s + "]==]"


def write_variant(name, sample, python, use_s):
    d = os.path.join(work, name)
    os.makedirs(d)
    lines = []
    for t in sample:
        cmd = list(t["command"])
        assert cmd[1] == "-S"
        cmd = [python] + (["-S"] if use_s else []) + cmd[2:]
        i = cmd.index("--cvc5-config-cache")
        cmd[i + 1] = os.path.join(d, "cvc5-show-config.json")
        env = list(prop(t, "ENVIRONMENT") or [])
        lines.append("add_test(%s %s)" % (bracket(t["name"]),
                                          " ".join(bracket(a) for a in cmd)))
        props = ["SKIP_RETURN_CODE 77"]
        if env:
            props.append("ENVIRONMENT " + bracket(
                ";".join(e.replace(";", r"\;") for e in env)))
        wd = prop(t, "WORKING_DIRECTORY")
        if wd:
            props.append("WORKING_DIRECTORY " + bracket(wd))
        lines.append("set_tests_properties(%s PROPERTIES %s)" % (
            bracket(t["name"]), " ".join(props)))
    with open(os.path.join(d, "CTestTestfile.cmake"), "w") as f:
        f.write("\n".join(lines) + "\n")
    return d


testers = "--tester base --tester model --tester synth --tester abduct"
variants = [("D_venv", venv_python, False, testers + " --tester dump"),
            ("E_base", base_python, False, testers + " --tester dump"),
            ("F_base_S", base_python, True, testers + " --tester dump"),
            ("G_base_S_nodump", base_python, True, testers)]
results = {}
for jobs, step in ((1, 14), (5, 7)):
    sample = tests[::step]
    for rnd, (name, python, use_s, args) in enumerate(variants + variants[::-1]):
        d = write_variant("%s_j%d_r%d" % (name, jobs, rnd), sample, python, use_s)
        env = dict(os.environ, CVC5_REGRESSION_ARGS="--no-early-exit",
                   RUN_REGRESSION_ARGS=args)
        s = time.perf_counter()
        r = subprocess.run(["ctest", "-j%d" % jobs], cwd=d, env=env,
                           capture_output=True, text=True)
        wall = time.perf_counter() - s
        times = [float(x) for x in re.findall(r"Passed +([\d.]+) sec", r.stdout)]
        summary = re.findall(r"\d+% tests passed.*", r.stdout)
        print("%-16s -j%d: wall %6.1f s, %s" % (name, jobs, wall,
              summary[0] if summary else r.stdout[-500:]))
        res = results.setdefault((name, jobs), {"wall": [], "times": []})
        res["wall"].append(wall)
        res["times"] += times

print("\n### %s" % label)
print("%-16s %4s %6s %16s %8s" % ("variant", "jobs", "tests", "wall (2 rounds)", "median"))
for jobs in (1, 5):
    for name, _, _, _ in variants:
        r = results[(name, jobs)]
        print("%-16s %4d %6d %6.1f %6.1f s %7.3f" % (
            name, jobs, len(r["times"]) // 2, r["wall"][0], r["wall"][1],
            statistics.median(r["times"]) if r["times"] else float("nan")))
