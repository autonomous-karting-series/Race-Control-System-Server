import { Card, Row, Col } from "react-bootstrap";
import "../App.css";

const TrackStatus = ({ track }) => {
  return (
    <Card.Body style={{ flex: "1 1 auto", overflow: "auto"}}>
      <div className="row align-items-center">
        <div className="col-auto dashboard-title">Track</div>
        <div
          className={`col-auto track-indicator flag-${track.state.toLowerCase()}`}
        />
      </div>
      <hr/>

      <Row sm={2} md={2} lg={3} xl={4} className='justify-content-md-center gap-2'>
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
      </Row>
    </Card.Body>
  );
};

export default TrackStatus;
