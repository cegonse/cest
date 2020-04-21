CEST_TEST_TEMPLATE = `#include <cest>

int sum(int a, int b) {
    return a + b;
}

describe("testing additions", []() {
    it("adds 1 + 2 to equal 3", []() {
        expect(sum(1, 2)).toBe(3);
    });
});`

DISABLED_RUN_BUTTON_INNER_HTML = '<i class="fa fa-spinner fa-spin"></i> Wait...'
ENABLED_RUN_BUTTON_INNER_HTML = 'Run!'

const buildPassedTest = (name) => {
    return `<div class="testResult">
        <span class="testPassed">PASS</span>
        <span class="testName">${name}</span>
    </div>`
}

const buildFailedTest = (name, message) => {
    return `<div class="testResult">
        <span class="testFailed">FAIL</span>
        <span class="testName">${name}</span>
        <p class="failureMessage">${message}</p>
    </div>`
}

const buildSkippedTest = (name) => {
    return `<div class="testResult">
        <span class="testSkipped">SKIP</span>
        <span class="testName">${name}</span>
    </div>`
}

var editor = null


class Presentation {
    static initializeCodeEditor() {
        editor = ace.edit('editor')
    
        editor.setTheme('ace/theme/xcode')
        editor.session.setMode('ace/mode/c_cpp')

        editor.insert(CEST_TEST_TEMPLATE)
    }

    static showErrorToast(message) {
        const error_toast = document.querySelector('#errorToast')
        const toast_message = document.querySelector('#toastMessage')

        toast_message.textContent = message
        error_toast.classList.remove('hiddenErrorToast')
    }

    static hideErrorToast() {
        const error_toast = document.querySelector('#errorToast')

        error_toast.classList.add('hiddenErrorToast')
    }

    static enableRunningTests() {
        const run_test_button = document.querySelector('#runTestButton')

        run_test_button.disabled = false
        run_test_button.innerHTML = ENABLED_RUN_BUTTON_INNER_HTML
    }

    static removeOldResults() {
        const results = document.querySelectorAll('.testResult')

        results.forEach((e) => {
            e.parentNode.removeChild(e)
        })
    }

    static addTestCaseResult(test_case) {
        const results_container = document.querySelector('#testResultsContainer')
        const new_node = document.createElement('div')

        if (test_case.failure_message) {
            new_node.innerHTML = buildFailedTest(test_case.name, test_case.failure_message)
        } else if (test_case.skipped) {
            new_node.innerHTML = buildSkippedTest(test_case.name)
        } else {
            new_node.innerHTML = buildPassedTest(test_case.name)
        }

        results_container.appendChild(new_node)
    }

    static renderResults(test_results) {
        Presentation.removeOldResults()

        test_results.test_cases.forEach((test => {
            Presentation.addTestCaseResult(test)
        }))
    }

    static onRunTestResponse(test_results) {
        Presentation.renderResults(test_results)
        Presentation.enableRunningTests()
    }

    static onRunTestFailure(problems) {
        var message = 'Something wrong happened!'

        if (problems) {
            if (problems.timed_out) {
                message = `Test execution timed out after ${problems.timed_out} seconds!`
            }

            if (problems.compilation) {
                message = 'Test compilation failed!'
            }
        }

        Presentation.showErrorToast(message)
        Presentation.enableRunningTests()
    }

    static onRunTestClicked() {
        const run_test_button = document.querySelector('#runTestButton')

        run_test_button.disabled = true
        run_test_button.innerHTML = DISABLED_RUN_BUTTON_INNER_HTML
        
        Api.runTest(editor.getValue())
    }
}


window.addEventListener('load', () => {
    const run_test_button = document.querySelector('#runTestButton')
    const close_toast_button = document.querySelector('#closeToast')

    window.bus.subscribe(Events.Api.RunTestResponse, Presentation.onRunTestResponse)
    window.bus.subscribe(Events.Api.RunTestFailure, Presentation.onRunTestFailure)

    run_test_button.addEventListener('click', Presentation.onRunTestClicked)
    close_toast_button.addEventListener('click', Presentation.hideErrorToast)

    Presentation.initializeCodeEditor()
})
