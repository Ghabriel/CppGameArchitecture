#include "engine/testing/core.hpp"

using test::describe;
using test::it;
using test::before;

void testEntitySystem() {
    describe("entity system", []() {
        it("placeholder test", []() {
            expect(true).toBe(true);
        });
    });
}

int main(int, char**) {
    testEntitySystem();
}
