# Το Makefile αυτό βρίσκεται στο root ολόκληρου του project και χρησιμεύει για
# να κάνουμε εύκολα compile πολλά τμήματα του project μαζί. Το Makefile αυτό
# καλεί το make στα διάφορα directories ως
#   $(MAKE) -C <dir> <target>
# το οποίο είναι ισοδύναμο με το να τρέξουμε make <target> μέσα στο directory <foo>

# Ολα τα directories μέσα στο programs directory
PROGRAMS = $(subst programs/, , $(wildcard programs/*))

# Compile: όλα, προγράμματα, tests
all: programs tests

# Η παρακάτω γραμμή δημιουργεί ένα target programs-<foo> για οποιοδήποτε <foo>. Η μεταβλητή $* περιέχει το "foo"
programs-%:
	$(MAKE) -C programs/$*

programs: $(addprefix programs-, $(PROGRAMS))		# depend στο programs-<foo> για κάθε στοιχείο του PROGRAMS

.PHONY: tests
tests:
	$(MAKE) -C tests all

# Εκτέλεση: όλα
run: run-programs run-tests

run-programs-%:
	$(MAKE) -C programs/$* run

run-programs: $(addprefix run-programs-, $(PROGRAMS))

run-tests:
	$(MAKE) -C tests run

# Εκτέλεση με valgrind: όλα, προγράμματα, tests
valgrind: valgrind-tests valgrind-programs

valgrind-programs-%:
	$(MAKE) -C programs/$* valgrind

valgrind-programs: $(addprefix valgrind-programs-, $(PROGRAMS))

valgrind-tests:
	$(MAKE) -C tests valgrind

# Εκκαθάριση
clean-programs-%:
	$(MAKE) -C programs/$* clean

clean: $(addprefix clean-programs-, $(PROGRAMS))
	$(MAKE) -C tests clean
