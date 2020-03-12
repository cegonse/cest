RUN_TEST_URL = 'http://localhost:3000/test'


class Api {
    static runTest(test_file) {
        fetch(RUN_TEST_URL, {
            method: 'POST',
            mode: 'cors',
            cache: 'no-cache',
            credentials: 'omit',
            headers: {
                'Content-Type': 'text/plain'
            },
            body: test_file
        }).then((data) => {
            data.json().then((results) => {
                if (results.problems) {
                    window.bus.publish(Events.Api.RunTestFailure, results.problems)        
                } else {
                    window.bus.publish(Events.Api.RunTestResponse, results)
                }
            })
        }).catch((reason) => {
            window.bus.publish(Events.Api.RunTestFailure, reason)
        })
    }
}
