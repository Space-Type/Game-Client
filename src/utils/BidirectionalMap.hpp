/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** BidirectionalMap class
*/

#pragma once

#include <unordered_map>


namespace RType::Utils {
    template <typename A, typename B>
    class BidirectionalMap {
        std::unordered_map<A, B> m_AtoB;
        std::unordered_map<B, A> m_BtoA;

        public:
            BidirectionalMap() noexcept
                : m_AtoB()
                , m_BtoA()
            {
            }

            /**
             * Associates A to B
             * and B to A.
             */
            void add(const A& a, const B& b)
            {
                m_AtoB[a] = b;
                m_BtoA[b] = a;
            }

            void remove(const A& a)
            {
                auto aIt = m_AtoB.find(a);
                if (aIt == m_AtoB.end())
                    return;

                m_BtoA.erase(aIt->second);
                m_AtoB.erase(aIt);
            }

            void remove(const B& b)
            {
                auto bIt = m_BtoA.find(b);
                if (bIt == m_BtoA.end())
                    return;

                m_AtoB.erase(bIt->second);
                m_BtoA.erase(bIt);
            }

            A& operator[](const B& value) { return m_BtoA.at(value); }
            const A& operator[](const B& value) const { return m_BtoA.at(value); }

            B& operator[](const A& value) { return m_AtoB.at(value); }
            const B& operator[](const A& value) const { return m_AtoB.at(value); }
    };
}
