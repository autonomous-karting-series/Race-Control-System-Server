// import React, {
//     useState,
//     useEffect
// } from 'react';
import { Card, ListGroup } from 'react-bootstrap';
const hash = require('object-hash');

const MQTTMessageList = (messages) => {
    if (messages)
    return (messages.map((message) => {
        return (
            <ListGroup.Item key={hash(message)} style={{padding: '2px 0px 0px 0px'}}>
                <div className="row align-items-start">
                    <div className="col-auto" style={{ minWidth: '100px', flexShrink: 0 }}>{message.topic}:</div>
                    <div className="col" style={{ minWidth: 0 }}>
                        <p style={{ wordWrap: 'break-word', overflowWrap: 'break-word', hyphens: 'auto' }}>{message.payload}</p>
                    </div>
                    <div className="col-auto text-end text-muted" style={{ flexShrink: 0 }}>{message.timestamp}</div>
                </div>
            </ListGroup.Item>
        );
    }));
}

const MQTTFeed = ({messages}) => {

    // const [messageFilter, setMessageFilter] = useState('');
    // const [topicFilter, setTopicFilter] = useState('');

    // useEffect(() => {
    //     console.log('someValue has changed:', messages);
    // }, [messages]);

    return (
        <Card.Body
            ref={(el) => {
                if (el) {
                el.scrollTop = el.scrollHeight;
                }
            }}
            style={{ flex: '1 1 auto', overflow: 'auto' }}>
            <ListGroup variant="flush">
                {MQTTMessageList(messages)}
                <ListGroup.Item>
                    <div className="col-auto text-start text-muted" style={{ flexShrink: 0 }}>...</div>
                </ListGroup.Item>
            </ListGroup>
        </Card.Body>
    );
};

export default MQTTFeed;
