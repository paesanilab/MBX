from __future__ import annotations

from pathlib import Path
import sys
import unittest


TEST_DIR = Path(__file__).resolve().parent


class ReportingTestResult(unittest.TextTestResult):
    """Collect explicit pass/fail outcomes for a compact post-run summary."""

    def __init__(self, stream, descriptions, verbosity):
        super().__init__(stream, descriptions, verbosity)
        self.passed = []
        self.failed = []
        self.errored = []
        self.skipped_tests = []
        self.expected_failures = []
        self.unexpected_successes_list = []

    def addSuccess(self, test):
        super().addSuccess(test)
        self.passed.append(test.id())

    def addFailure(self, test, err):
        super().addFailure(test, err)
        self.failed.append((test.id(), self._exc_info_to_string(err, test)))

    def addError(self, test, err):
        super().addError(test, err)
        self.errored.append((test.id(), self._exc_info_to_string(err, test)))

    def addSkip(self, test, reason):
        super().addSkip(test, reason)
        self.skipped_tests.append((test.id(), reason))

    def addExpectedFailure(self, test, err):
        super().addExpectedFailure(test, err)
        self.expected_failures.append((test.id(), self._exc_info_to_string(err, test)))

    def addUnexpectedSuccess(self, test):
        super().addUnexpectedSuccess(test)
        self.unexpected_successes_list.append(test.id())


def build_suite():
    loader = unittest.defaultTestLoader
    return loader.discover(start_dir=str(TEST_DIR), pattern="test_*.py", top_level_dir=str(TEST_DIR))


def print_summary(result):
    print("\nASE patch test summary:")

    for test_id in result.passed:
        print(f"PASS  {test_id}")

    for test_id, _ in result.skipped_tests:
        print(f"SKIP  {test_id}")

    for test_id, _ in result.failed:
        print(f"FAIL  {test_id}")

    for test_id, _ in result.errored:
        print(f"ERROR {test_id}")

    for test_id, _ in result.expected_failures:
        print(f"XFAIL {test_id}")

    for test_id in result.unexpected_successes_list:
        print(f"XPASS {test_id}")

    print(
        "\nTotals: "
        f"run={result.testsRun}, "
        f"passed={len(result.passed)}, "
        f"failed={len(result.failed)}, "
        f"errors={len(result.errored)}, "
        f"skipped={len(result.skipped_tests)}, "
        f"xfail={len(result.expected_failures)}, "
        f"xpass={len(result.unexpected_successes_list)}"
    )

    if result.failed or result.errored or result.unexpected_successes_list:
        print("\nFailure details:")
        for test_id, detail in result.failed:
            print(f"\n[test failure] {test_id}\n{detail}")
        for test_id, detail in result.errored:
            print(f"\n[test error] {test_id}\n{detail}")


def main():
    suite = build_suite()
    runner = unittest.TextTestRunner(
        stream=sys.stdout,
        verbosity=0,
        resultclass=ReportingTestResult,
    )
    result = runner.run(suite)
    print_summary(result)
    return 0 if result.wasSuccessful() else 1


if __name__ == "__main__":
    raise SystemExit(main())
