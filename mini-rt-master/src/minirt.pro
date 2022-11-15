TEMPLATE = subdirs

#CONFIG += ordered

SUBDIRS = \
    minirt \
    test \
    test_anim

minirt.subdir = minirt
test.subdir = test
test_anim.subdir = test_anim

test.depends = minirt
test_anim.depends = minirt


