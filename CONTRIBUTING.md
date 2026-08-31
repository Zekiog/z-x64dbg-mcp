# Contributing

Thanks for taking the time to contribute. This document explains how to
report issues, propose changes, and submit pull requests that will be
reviewed and merged quickly.

## Code of Conduct

By participating in this project you agree to keep the interaction
respectful, technical, and on-topic. Harassment, off-topic promotion, or
deliberately hostile comments will be removed without notice.

## Before opening an issue

1. Search existing issues (open + closed) for the same problem.
2. Read the README and `docs/` (if any) — most setup questions are
   already answered there.
3. Try the latest `main`. Many reported issues are already fixed.

## Reporting bugs

Use the **Bug report** issue template. Include:

- Reproduction steps (minimal, copy-pastable)
- Expected vs actual behaviour
- Environment (OS, language runtime version, relevant service versions)
- Relevant logs / stack traces — redact secrets before pasting

## Proposing features

Use the **Feature request** issue template. Explain:

- The problem you are trying to solve
- The proposed solution (one sentence if possible)
- Alternatives you considered
- Whether you are willing to send a PR

## Submitting a pull request

1. Fork the repository and create a feature branch from `main`.
2. Make focused commits. Each commit should compile / pass tests.
3. Follow the existing code style. Run formatters and linters locally
   before pushing.
4. Add or update tests for behavioural changes.
5. Update the README / docs if user-visible behaviour changes.
6. Open a PR against `main` using the PR template. Fill in:
   - **What** changed (one paragraph)
   - **Why** (link the issue it closes, e.g. `Closes #42`)
   - **How** to verify locally (commands, screenshots)

CI must be green before a review is requested. The reviewer may ask for
small follow-ups — these are not rejections.

## Security issues

**Do not** open a public issue for security vulnerabilities. Follow the
process in [SECURITY.md](./SECURITY.md) instead.

## Licensing

By submitting a contribution you agree that it will be licensed under
the same license as the repository (see `LICENSE`).
