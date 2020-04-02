RUN_TEST_URL = 'https://api.cestframework.com:3322/test'


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
            if (data.status == 200) {
                data.json().then((results) => {
                    window.bus.publish(Events.Api.RunTestResponse, results)
                })
            } else {
                data.json().then((results) => {
                    window.bus.publish(Events.Api.RunTestFailure, results.problems)
                }).catch((reason) => {
                    window.bus.publish(Events.Api.RunTestFailure, null)
                })
            }
        }).catch((reason) => {
            window.bus.publish(Events.Api.RunTestFailure, null)
        })
    }
}
