import { Card, Row, Col, ListGroup, Button, ButtonGroup } from "react-bootstrap";
import "../App.css";

const RCSControls = ({ rcs }) => {


  return (
    <Card.Body style={{ flex: "1 1 auto", overflow: "auto" }}>
      {/* <div className="row align-items-center">
        <div className="col-auto dashboard-title">Track</div>
        <div
          className={`col-auto track-indicator flag-${track.state.toLowerCase()}`}
        />
      </div>
      <hr/>

      <Row sm={2} md={2} lg={3} xl={4} className="justify-content-md-center">
        {track.sectors.map((sector_state, sector_id) => {
          return (
            <Col key={sector_id}>
              <div>Sector {sector_id + 1}</div>
              <div
                className={`flag-indicator flag-${sector_state.toLowerCase()}`}
              />
            </Col>
          );
        })}
      </Row> */}

      <ListGroup variant="flush">

        <ListGroup.Item
          key="state"
          as="li"
          className="status-item"
          style={{ width: "100%" }}
        >
          <div className="flex-fill">Current State: <b>{rcs.state}</b></div>

        </ListGroup.Item>
        <ListGroup.Item
          key="rc"
          as="li"
          className="status-item"
          style={{ width: "100%" }}
        >
          <Button variant='success'>Enable RC</Button>
          <Button variant='danger'>Cancel RC</Button>

        </ListGroup.Item>
      </ListGroup>

      <Row sm={2} className='justify-content-md-center'>
        {/* <Button>Initiate</Button> */}
        <Button variant='warning'>Enable RC</Button>
        <Button variant='danger'>Cancel RC</Button>
        <Button variant='success'>Enable Autonomous</Button>
        <Button variant='danger'>Cancel Autonomous</Button>
        <Button></Button>
        <Button></Button>

      </Row>
    </Card.Body>
  );
};

export default RCSControls;
