describe("Event Bus", function() {
    var bus;
    var on_event_called;
    var on_event_data;
    var on_other_event_called;
    var on_other_event_data;

    let onEvent = function(data) { on_event_called = true; on_event_data = data }
    let onOtherEvent = function(data) { on_other_event_called = true; on_other_event_data = data }
  
    beforeEach(function() {
        bus = new EventBus()

        on_event_called = false
        on_other_event_called = false
        on_event_data = null
        on_other_event_data = null
    });
  
    it("allows subscription to an event", function() {
        bus.subscribe('event', onEvent)
        bus.publish('event', 'data')

        expect(on_event_called).toBe(true)
        expect(on_event_data).toBe('data')
    })

    it("allows subscription to many events", function() {
        bus.subscribe('event', onEvent)
        bus.subscribe('other event', onOtherEvent)

        bus.publish('event', 'data')
        bus.publish('other event', 'other data')

        expect(on_event_called).toBe(true)
        expect(on_other_event_called).toBe(true)
        expect(on_event_data).toBe('data')
        expect(on_other_event_data).toBe('other data')
    })
})
