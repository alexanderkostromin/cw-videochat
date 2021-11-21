#ifndef ROOM_SERVICE_HPP
#define ROOM_SERVICE_HPP

#include "../db/DbClient.hpp"
#include "../dto/OfferDto.hpp"

#include "oatpp/web/protocol/http/Http.hpp"
#include "oatpp/core/macro/component.hpp"
#include <string>
#include <iostream>

class RoomService{
private:
	typedef oatpp::web::protocol::http::Status Status;
private:
	OATPP_COMPONENT(std::shared_ptr<DbClient>, database); // Inject database component
public:

	oatpp::Object<RoomDto> createRoom() {
		auto result = database->createRoom();
		auto roomId = oatpp::sqlite::Utils::getLastInsertRowId(result->getConnection());
		return getById((v_int32) roomId);
	};
	oatpp::Object<RoomDto> getById(const oatpp::Int32& id) {
		auto result = database->getById(id);

		OATPP_ASSERT_HTTP(result->hasMoreToFetch(), Status::CODE_404, "Not found");
		auto resultVector = result->fetch<oatpp::Vector<oatpp::Object<RoomDto>>>();
		return resultVector[0];
	}
	oatpp::Object<RoomDto> setAnswer(const oatpp::Object<AnswerDto>& dto) {
		auto result = database->executeQuery("UPDATE Room SET answer=:answer WHERE id=:id", {
				{"id", oatpp::Int32(dto->id)},
				{"answer", oatpp::String(dto->answer)}
		});
		return getById((v_int32) dto->id);
	};

	oatpp::Object<RoomDto> setOffer(const oatpp::Object<OfferDto>& dto) {
		auto result = database->setOffer(dto);
		return getById((v_int32) dto->id);
	};

	oatpp::Object<RoomDto> setIceCandidate(const oatpp::Object<CandidateDto>& dto) {
		oatpp::String column;
		dto->creator ? column = "ice_candidate_offer" : column = "ice_candidate_answer";
		oatpp::String query = "UPDATE Room SET " + column + "=:ice_candidate WHERE id=:id";
		auto result = database->executeQuery(query, {
				{"id", oatpp::Int32(dto->id)},
				{"ice_candidate", oatpp::String(dto->ice_candidate)}
		});
		return getById((v_int32) dto->id);
	};
//	oatpp::Object<UrlDto> getUrlById(const oatpp::Int32& id, const std::shared_ptr<oatpp::orm::Connection>& connection = nullptr);
//	oatpp::Object<UrlDto> getByShorten(const oatpp::String& shorten, const std::shared_ptr<oatpp::orm::Connection>& connection = nullptr);
};

#endif ROOM_SERVICE_HPP