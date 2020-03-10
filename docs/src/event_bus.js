class EventBus {
    constructor() {
        this.subscriptions = []
    }

    publish(event, data) {
        for (const subscription of this.subscriptions) {
            if (subscription.event.match(event)) {
                subscription.callback(data)
            }
        }
    }

    subscribe(event, callback) {
        this.subscriptions.push({
            event: event,
            callback: callback
        })
    }
}
