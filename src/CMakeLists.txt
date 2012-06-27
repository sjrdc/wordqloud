include_directories(
  )

ADD_EXECUTABLE(wordcloud
  word.cpp
  main.cpp
  )
TARGET_LINK_LIBRARIES(wordcloud
  ${Boost_LIBRARIES}
  ${QT_LIBRARIES}
  )

install(TARGETS
  wordcloud
  RUNTIME DESTINATION bin/linaccontrolclients
  )

