// Includes

#include "main.h"
#include "RoomWidget.h"

// event_handler_send_event

uint8_t event_handler_send_event(APP_DATA*          data,
                                struct RoomWidget* widget,
                                uint8_t            event)
{
    uint8_t i;

    if ( !data || !data->event_callback )
        return(RESULT_ERROR);

    // Notify remote device.

    if ( event == EVENT_ROOM_WIDGET_UPDATED_LOCALLY )
    {

        room_configuration_notify_widget_updated(data, widget);
        return(RESULT_OK);
    }

    // Notify UI.

    for ( i= 0; i < data->event_callback_count; i++ )
    {
        if ( data->event_callback[i] )
            data->event_callback[i](data, widget, event);
    }

    return(RESULT_OK);
}


// set_application_event_callback

void event_handler_register_callback(APP_DATA*     data,
		                                EventCallback c)
{
    if ( !data || data->event_callback_count >= MAX_EVENT_CALLBACK_COUNT-1 || !c )
        return;

	data->event_callback[data->event_callback_count]= c;
    data->event_callback_count++;
}

// event_handler_unregister_callback

void event_handler_unregister_callback(APP_DATA*     data,
		                               EventCallback c)
{
    uint8_t i;

    if ( !data || data->event_callback_count == 0 || !c )
        return;

    for ( i= 0; i < data->event_callback_count; i++ )
    {
        if ( c == data->event_callback[i] )
        {
            data->event_callback[i]= 0;
            data->event_callback_count--;
            break;
        }
    }
}
