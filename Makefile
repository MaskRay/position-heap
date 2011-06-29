CXXFLAGS := -g
prog := fastBuildFastSearch fastBuildNaiveSearch naiveBuildFastSearch naiveBuildNaiveSearch

define test
	@(tr -cd a-d < /dev/urandom | head -c 1000; echo; tr -cd a-d < /dev/urandom | head -c 3) > /tmp/in
	bash -c 'diff -B <(./$(@:test_%=%) < /tmp/in | xargs -n 1 | sort -n) <(./all-occurrences < /tmp/in)'

	@(tr -cd a-c < /dev/urandom | head -c 2000; echo; tr -cd a-d < /dev/urandom | head -c 5) > /tmp/in
	bash -c 'diff -B <(./$(@:test_%=%) < /tmp/in | xargs -n 1 | sort -n) <(./all-occurrences < /tmp/in)'
endef

define NL


endef

fastBuildFastSearch: main.cpp
	$(CXX) $(CXXFLAGS) -D LINEAR_BUILD -D LINEAR_SEARCH $< -o $@

fastBuildNaiveSearch: main.cpp
	$(CXX) $(CXXFLAGS) -D LINEAR_BUILD $< -o $@

naiveBuildFastSearch: main.cpp
	$(CXX) $(CXXFLAGS) -D LINEAR_SEARCH $< -o $@

naiveBuildNaiveSearch: main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(foreach i,$(prog),$(eval test_$i: $i$(NL)	$$(test)))

test_all: $(foreach i,$(prog),test_$i)
