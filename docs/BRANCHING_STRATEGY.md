# Branching Strategy

This document provides a detailed reference for the Cellular Threshold branching strategy.

## Quick Reference

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                         BRANCH OVERVIEW                                      │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                              │
│  main ────────●────────────────────●────────────────●───────────            │
│               │ (v1.0.0)           │ (v1.0.1)       │ (v1.1.0)              │
│               │                    │                │                        │
│  release/* ───│────────────────────│────────●───────│                        │
│               │                    │        │       │                        │
│  hotfix/*  ───│────────────────────●────────│───────│                        │
│               │                             │       │                        │
│  develop  ────●────●────●────●────●────●────●───────●────●────●─────────    │
│               │    │    │    │                      │    │                   │
│  feature/* ───│────●    │    ●────●                 │    ●────●              │
│               │         │                           │                        │
│  bugfix/*  ───│─────────●                           │                        │
│                                                                              │
└─────────────────────────────────────────────────────────────────────────────┘
```

## Branch Lifecycle

### Feature Branch

```
Timeline: develop ──●──────────────────●── develop
                    │                  │
                    └── feature/xyz ───┘

Duration: Days to weeks
Created from: develop
Merged into: develop
Deleted: After merge
```

**Commands:**
```bash
# Start
git checkout develop
git pull origin develop
git checkout -b feature/my-feature

# During development
git add .
git commit -m "feat(scope): description"
git push -u origin feature/my-feature

# Keep up to date
git fetch origin develop
git rebase origin/develop  # or merge

# Finish (via PR)
# After PR approved and merged:
git checkout develop
git pull origin develop
git branch -d feature/my-feature
git push origin --delete feature/my-feature
```

### Release Branch

```
Timeline: develop ──●─────────●── develop
                    │         │
                    └── release/1.0.0 ──●── main
                                        │
                                        └── tag: v1.0.0

Duration: Days (stabilization only)
Created from: develop
Merged into: main AND develop
Deleted: After merge
```

**Commands:**
```bash
# Start release
git checkout develop
git checkout -b release/1.0.0

# Only bug fixes and version updates allowed
echo "1.0.0" > VERSION
git commit -am "chore(release): prepare v1.0.0"

# After stabilization, merge to main
git checkout main
git merge --no-ff release/1.0.0 -m "chore(release): v1.0.0"
git tag -a v1.0.0 -m "Release v1.0.0"
git push origin main --tags

# Merge back to develop
git checkout develop
git merge --no-ff release/1.0.0 -m "chore: merge release/1.0.0 back to develop"
git push origin develop

# Cleanup
git branch -d release/1.0.0
git push origin --delete release/1.0.0
```

### Hotfix Branch

```
Timeline: main ────●─────────●── main
                   │ (v1.0)  │ (v1.0.1)
                   │         │
                   └── hotfix/1.0.1 ──┬── main
                                      │
                                      └── develop

Duration: Hours to days (urgent)
Created from: main (latest tag)
Merged into: main AND develop
Deleted: After merge
```

**Commands:**
```bash
# Start hotfix from main
git checkout main
git checkout -b hotfix/1.0.1

# Make the fix
git commit -am "fix(scope): critical bug description"

# Update version
echo "1.0.1" > VERSION
git commit -am "chore(release): bump to v1.0.1"

# Merge to main
git checkout main
git merge --no-ff hotfix/1.0.1 -m "chore(release): v1.0.1"
git tag -a v1.0.1 -m "Hotfix v1.0.1"
git push origin main --tags

# Merge to develop (important!)
git checkout develop
git merge --no-ff hotfix/1.0.1 -m "chore: merge hotfix/1.0.1 to develop"
git push origin develop

# Cleanup
git branch -d hotfix/1.0.1
```

## Branch Protection Rules

### `main` Branch

| Rule | Setting |
|------|---------|
| Require PR | Yes |
| Required approvals | 1+ |
| Dismiss stale reviews | Yes |
| Require status checks | Yes |
| Require up-to-date | Yes |
| Include administrators | Yes |
| Allow force push | No |
| Allow deletions | No |

### `develop` Branch

| Rule | Setting |
|------|---------|
| Require PR | Yes |
| Required approvals | 1 |
| Require status checks | Yes |
| Allow force push | No |

## CI Triggers

| Branch Pattern | CI Workflow | Release Workflow |
|----------------|-------------|------------------|
| `main` | ✓ Build + Test | ✓ On tag |
| `develop` | ✓ Build + Test | ✗ |
| `feature/*` | ✓ Build + Test | ✗ |
| `bugfix/*` | ✓ Build + Test | ✗ |
| `release/*` | ✓ Build + Test | ✗ |
| `hotfix/*` | ✓ Build + Test | ✗ |
| PR to main | ✓ Full suite | ✗ |
| PR to develop | ✓ Full suite | ✗ |

## Common Scenarios

### Scenario 1: Starting a New Feature

```bash
# Always start from latest develop
git checkout develop
git pull origin develop

# Create feature branch
git checkout -b feature/entity-component-system

# Make commits following conventional commit format
git commit -m "feat(ecs): add entity creation"
git commit -m "feat(ecs): implement component storage"
git commit -m "test(ecs): add entity tests"

# Push and create PR
git push -u origin feature/entity-component-system
# Create PR via GitHub to develop
```

### Scenario 2: Preparing a Release

```bash
# Ensure develop is stable
git checkout develop
git pull origin develop

# Create release branch
git checkout -b release/1.2.0

# Only these types of changes allowed:
# - Bug fixes
# - Documentation
# - Version bumps

# Update version
sed -i 's/VERSION .*/VERSION 1.2.0/' CMakeLists.txt
echo "1.2.0" > VERSION
git commit -am "chore(release): prepare v1.2.0"

# Test thoroughly, fix any issues found
git commit -m "fix(render): resolve texture loading issue"

# When ready, merge and tag
```

### Scenario 3: Emergency Production Fix

```bash
# Production is broken! Start from main
git checkout main
git pull origin main

# Create hotfix
git checkout -b hotfix/1.1.1

# Fix the issue
git commit -m "fix(vulkan): prevent crash on device loss"

# Test thoroughly
# Update version and merge ASAP
```

### Scenario 4: Resolving Merge Conflicts

```bash
# Feature branch has conflicts with develop
git checkout feature/my-feature
git fetch origin develop

# Option A: Rebase (cleaner history)
git rebase origin/develop
# Resolve conflicts in each commit
git add <resolved-files>
git rebase --continue

# Option B: Merge (preserves history)
git merge origin/develop
# Resolve conflicts
git add <resolved-files>
git commit
```

## Version Numbering

### Semantic Versioning: MAJOR.MINOR.PATCH

| Change Type | Version Bump | Example |
|-------------|--------------|---------|
| Breaking API change | MAJOR | 1.0.0 → 2.0.0 |
| New feature | MINOR | 1.0.0 → 1.1.0 |
| Bug fix | PATCH | 1.0.0 → 1.0.1 |

### Pre-release Versions

| Stage | Format | Example |
|-------|--------|---------|
| Alpha | X.Y.Z-alpha.N | 1.0.0-alpha.1 |
| Beta | X.Y.Z-beta.N | 1.0.0-beta.1 |
| Release Candidate | X.Y.Z-rc.N | 1.0.0-rc.1 |

### Automatic Version Detection

The CI system analyzes commits to determine version bumps:

```
feat: ...     → MINOR bump
fix: ...      → PATCH bump
feat!: ...    → MAJOR bump
BREAKING CHANGE: → MAJOR bump
```

## FAQ

**Q: Should I rebase or merge to update my feature branch?**
A: Prefer rebase for cleaner history, but merge is acceptable if rebase causes issues.

**Q: Can I push directly to develop?**
A: No, all changes require a PR.

**Q: What if my feature isn't ready but I need to switch branches?**
A: Use `git stash` or commit with `WIP:` prefix (squash before PR).

**Q: How long should a feature branch live?**
A: Ideally less than 2 weeks. Break large features into smaller PRs.
