# NOTE: Greedy, may eat up your RAM
from pkgcore.ebuild.cpv import CPV
import os, time

gentoo_repo = '/var/gentoo/repos/gentoo'

def portage_ebuilds(repo):
    categories = os.walk(repo).next()[1]
    categories.remove('.git')
    categories.remove('metadata')
    esuffix = len('.ebuild')
    repo_ebuilds = []
    for cat in categories:
        for dirpath, dirnames, files in os.walk(repo + "/" + cat):
            for f in files:
                if f.endswith('.ebuild'):
                    repo_ebuilds.append(cat + '/' + f[:-esuffix])
    return repo_ebuilds

def generate_ebuilds(repo):
    categories = os.walk(repo).next()[1]
    categories.remove('.git')
    categories.remove('metadata')
    packages = []
    for cat in categories:
        packages.extend(os.walk(repo + "/" + cat).next()[1])
    versions = [str(i) + '.' + str(j) for i in range(3) for j in range(3)]

    gen_ebuilds = []
    for cat in categories:
        for pkg in packages:
            for ver in versions:
                gen_ebuilds.append(cat + '/' + pkg + '-' + ver)
    return gen_ebuilds

ebuilds = portage_ebuilds(gentoo_repo)

total = 0
n = 10
print('fire it')
for i in range(n):
    start = time.time()
    for pkg in ebuilds:
        CPV(pkg, versioned=True)
    end = time.time()
    print('%s' % (end - start))
    total = total + end - start

print('average %s' % (total / n))
