CEST_TEST_TEMPLATE = `#include <cest>

int sum(int a, int b) {
    return a + b;
}

describe("testing additions", []() {
    it("adds 1 + 2 to equal 3", []() {
        expect(sum(1, 2)).toBe(3);
    });
});`


function onDocumentLoad() {
    const editor = ace.edit('editor')
    
    editor.setTheme('ace/theme/xcode')
    editor.session.setMode('ace/mode/c_cpp')

    editor.insert(CEST_TEST_TEMPLATE)
}


window.addEventListener('load', onDocumentLoad)
